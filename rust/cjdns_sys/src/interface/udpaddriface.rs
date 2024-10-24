use num_enum::{IntoPrimitive, TryFromPrimitive};
use socket2::{Domain, Protocol, SockAddr, Type};
use tokio::net::UdpSocket;
use tokio::sync::mpsc::{Receiver, Sender};
use tokio::sync::Mutex;
use crate::util::sockaddr::Sockaddr;
use std::convert::TryFrom;
use std::sync::atomic::AtomicI32;
use std::sync::Arc;
use crate::interface::wire::message::Message;
use crate::external::interface::iface::{self, IfRecv, Iface, IfacePvt};
use anyhow::{Context, Result};
use std::net::SocketAddr;

const SEND_MSGS_LIMIT: usize = 16;
const TO_GO_OUT_QUEUE: usize = 64;

const BUFFER_CAP: usize = 3496;
const PADDING_AMOUNT: usize = 512;

#[derive(Debug,IntoPrimitive,TryFromPrimitive)]
#[repr(i32)]
pub enum SendWorkerState {
    Invalid = -1,
    Initializing = 0,
    WaitLock = 1,
    RecvBatch = 2,
    SendBatch = 3,
}

#[derive(Debug,IntoPrimitive,TryFromPrimitive)]
#[repr(i32)]
pub enum RecvWorkerState {
    Invalid = -1,
    Initializing = 0,
    RecvBatch = 1,
    RecievedBatch = 2,
    IfaceSendOne = 3,
    IfaceSendTwo = 4,
}

struct UDPAddrIfaceInternal {
    iface: IfacePvt,
    udp: UdpSocket,
    to_go_out_recv: Mutex<Receiver<(Message,SocketAddr)>>,
    to_go_out_send: Sender<(Message,SocketAddr)>,

    send_worker_states: Vec<AtomicI32>,
    recv_worker_states: Vec<AtomicI32>,
}
impl IfRecv for Arc<UDPAddrIfaceInternal> {
    fn recv(&self, mut m: Message) -> Result<()> {
        let sa =
            Sockaddr::try_from(m.bytes()).context("Getting address from message")?;
        m.discard_bytes(sa.byte_len())?;
        let sa = sa.rs()?;
        self.to_go_out_send.try_send((m, sa)).context("Not enough buffer space to send pkt")?;
        Ok(())
    }
}
impl UDPAddrIfaceInternal {
    fn send_worker_set_state(self: &Arc<Self>, n: usize, state: SendWorkerState) {
        self.send_worker_states[n].store(state as i32, std::sync::atomic::Ordering::Relaxed);
    }
    async fn send_worker(self: Arc<Self>, n: usize) {
        let mut send_msgs = Vec::with_capacity(SEND_MSGS_LIMIT);
        loop {
            self.send_worker_set_state(n, SendWorkerState::WaitLock);
            let mut tgo = self.to_go_out_recv.lock().await;
            self.send_worker_set_state(n, SendWorkerState::RecvBatch);
            tgo.recv_many(&mut send_msgs, SEND_MSGS_LIMIT).await;
            drop(tgo);
            self.send_worker_set_state(n, SendWorkerState::SendBatch);
            for (msg, sa) in send_msgs.drain(..) {
                // println!("got message with length: {}", msg.len());
                let bytes = msg.bytes();
                match self.udp.send_to(bytes, &sa).await {
                    Ok(_) => {
                        log::trace!("Message to {sa} sent ok");
                    },
                    Err(e) => {
                        log::info!("Unable to send message (len: {}): {e} to: {}",
                            bytes.len(), sa);
                    }
                }
            }
        }
    }
    fn recv_worker_set_state(self: &Arc<Self>, n: usize, state: RecvWorkerState) {
        self.recv_worker_states[n].store(state as i32, std::sync::atomic::Ordering::Relaxed);
    }
    async fn recv_worker(self: Arc<Self>, n: usize) {
        let mut message = None;
        loop {
            // If we get the lock on the receive channel, drain it and send back until it's empty
            // Allocate a Message
            let mut msg = if let Some(msg) = message.take() {
                msg
            } else {
                let mut msg = Message::new(PADDING_AMOUNT + BUFFER_CAP);
                msg.allocate_uninitialized(BUFFER_CAP).unwrap();
                msg
            };
            self.recv_worker_set_state(n, RecvWorkerState::RecvBatch);
            let res = self.udp.recv_from(msg.bytes_mut()).await;
            self.recv_worker_set_state(n, RecvWorkerState::RecievedBatch);
            log::trace!("recv_worker got a recv_from");
            match res {
                Ok((byte_count,from)) => {
                    log::trace!("Ok UDP packet from {from} with {byte_count} bytes");
                    if byte_count == BUFFER_CAP {
                        log::warn!("Truncated incoming message from {from}");
                    }
                    msg.set_len(byte_count).unwrap();
                    let addr = Sockaddr::from(&from);
                    msg.push_bytes(addr.bytes()).unwrap();
                    match self.iface.send(msg) {
                        Ok(()) => {
                            log::trace!("UDP receiver thread sent packet successfully");
                        },
                        Err(e) => {
                            log::debug!("Error processing packet: {e}");
                        }
                    }
                }
                Err(e) => {
                    log::warn!("Error receiving UDP message {e}");
                }
            }
        }
    }
}


pub struct UDPAddrIface {
    internal: Arc<UDPAddrIfaceInternal>,
    pub local_addr: SocketAddr,
}

impl UDPAddrIface {
    pub fn new(bind_addr: &SocketAddr) -> Result<(Self,Iface)> {
        let udp = socket2::Socket::new(
            Domain::for_address(bind_addr.clone()),
            Type::DGRAM,
            Some(Protocol::UDP),
        )?;
        udp.set_nonblocking(true)?;
        udp.set_reuse_address(true)?;
        let sa = SockAddr::from((*bind_addr).clone());
        udp.bind(&sa)?;
        let udp = UdpSocket::from_std(udp.into())?;
        let real_addr = udp.local_addr()?;
        let (mut iface, iface_pvt) = iface::new("UDPAddrIface");
        let (tgo, tgo_r) =
            tokio::sync::mpsc::channel(TO_GO_OUT_QUEUE);

        let workers = num_cpus::get();
        let workers = if workers < 2 {
            log::warn!("UDPAddrIface WORKERS = {workers} is too few, using 2");
            2
        } else {
            workers
        };

        let internal = Arc::new(UDPAddrIfaceInternal {
            iface: iface_pvt,
            udp,
            to_go_out_recv: Mutex::new(tgo_r),
            to_go_out_send: tgo,
            send_worker_states: (0..workers).map(|_|AtomicI32::new(0)).collect(),
            recv_worker_states: (0..workers).map(|_|AtomicI32::new(0)).collect(),
        });
        iface.set_receiver(Arc::clone(&internal));

        for i in 0..workers {
            tokio::task::spawn(Arc::clone(&internal).recv_worker(i));
            tokio::task::spawn(Arc::clone(&internal).send_worker(i));
        }
        Ok((
            UDPAddrIface{
                internal,
                local_addr: real_addr,
            },
            iface
        ))
    }

    pub fn set_dscp(&self, dscp: u8) -> Result<()> {
        let local_addr = self.internal.udp.local_addr()?;
        let tos = (dscp as u32) << 2;
        if local_addr.is_ipv6() {
            socket2::SockRef::from(&self.internal.udp).set_tclass_v6(tos)?;
        } else {
            self.internal.udp.set_tos(tos)?;
        }
        Ok(())
    }

    pub fn set_broadcast(&self, enable: bool) -> Result<()> {
        self.internal.udp.set_broadcast(enable)?;
        Ok(())
    }

    #[cfg(os = "windows")]
    pub fn get_fd(&self) -> u32 {
        -1
    }

    #[cfg(not(os = "windows"))]
    pub fn get_fd(&self) -> i32 {
        use std::os::fd::AsFd;
        use std::os::fd::AsRawFd;
        let bfd = self.internal.udp.as_fd();
        bfd.as_raw_fd()
    }

    pub fn worker_states(&self) -> (Vec<SendWorkerState>,Vec<RecvWorkerState>) {
        let mut rout = Vec::with_capacity(self.internal.recv_worker_states.len());
        let mut sout = Vec::with_capacity(self.internal.send_worker_states.len());
        for r in &self.internal.recv_worker_states {
            let n = r.load(std::sync::atomic::Ordering::Relaxed);
            let x = match RecvWorkerState::try_from(n) {
                Ok(x) => x,
                Err(_) => RecvWorkerState::Invalid,
            };
            rout.push(x);
        }
        for s in &self.internal.send_worker_states {
            let n = s.load(std::sync::atomic::Ordering::Relaxed);
            let x = match SendWorkerState::try_from(n) {
                Ok(x) => x,
                Err(_) => SendWorkerState::Invalid,
            };
            sout.push(x);
        }
        (sout, rout)
    }
}