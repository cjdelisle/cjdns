use socket2::{Domain, Protocol, Type};
use tokio::net::UdpSocket;
use tokio::sync::mpsc::{Receiver, Sender};
use tokio::sync::Mutex;
use crate::util::sockaddr::Sockaddr;
use std::convert::TryFrom;
use std::sync::Arc;
use crate::interface::wire::message::Message;
use crate::external::interface::iface::{self, IfRecv, Iface, IfacePvt};
use anyhow::{Context, Result};
use std::net::SocketAddr;

const SEND_MSGS_LIMIT: usize = 16;
const RECV_MSGS_LIMIT: usize = 16;

const TO_GO_OUT_QUEUE: usize = 64;
const INCOMING_QUEUE: usize = 64;

const BUFFER_CAP: usize = 3496;
const PADDING_AMOUNT: usize = 512;

const WORKERS: usize = 16;

struct UDPAddrIfaceInternal {
    iface: IfacePvt,
    udp: UdpSocket,
    to_go_out_recv: Mutex<Receiver<(Message,SocketAddr)>>,
    to_go_out_send: Sender<(Message,SocketAddr)>,

    incoming_recv: Mutex<Receiver<Message>>,
    incoming_send: Sender<Message>,
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
    async fn send_worker(self: Arc<Self>) {
        let mut send_msgs = Vec::with_capacity(SEND_MSGS_LIMIT);
        loop {
            let mut tgo = self.to_go_out_recv.lock().await;
            tgo.recv_many(&mut send_msgs, SEND_MSGS_LIMIT).await;
            drop(tgo);    
            for (msg, sa) in send_msgs.drain(..) {
                // println!("got message with length: {}", msg.len());
                let bytes = msg.bytes();
                match self.udp.send_to(bytes, sa).await {
                    Ok(_) => {
                        // println!("sent message ok");
                    },
                    Err(e) => {
                        log::info!("Unable to send message (len: {}): {e} to: {}",
                            bytes.len(), sa);
                    }
                }
            }
        }
    }
    async fn recv_worker(self: Arc<Self>) {
        let mut recv_msgs = Vec::with_capacity(RECV_MSGS_LIMIT);
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
            tokio::select! {
                res = self.udp.recv_from(msg.bytes_mut()) => {
                    log::debug!("recv_worker got a recv_from");
                    match res {
                        Ok((byte_count,from)) => {
                            log::debug!("Ok UDP packet from {from} with {byte_count} bytes");
                            if byte_count == BUFFER_CAP {
                                log::warn!("Truncated incoming message from {from}");
                            }
                            msg.set_len(byte_count).unwrap();
                            let addr = Sockaddr::from(&from);
                            msg.push_bytes(addr.bytes()).unwrap();
                            match self.incoming_send.send(msg).await {
                                Ok(()) => {
                                    log::debug!("UDP packet forwarded to receiver");
                                }
                                Err(e) => {
                                    log::info!("Lost message from {from} because: {e}");
                                }
                            }
                        }
                        Err(e) => {
                            log::warn!("Error receiving UDP message {e}");
                        }
                    }
                }
                mut recv = self.incoming_recv.lock() => {
                    message = Some(msg);
                    log::trace!("{:?} UDP receiver thread waiting", self.udp.local_addr());
                    recv.recv_many(&mut recv_msgs, RECV_MSGS_LIMIT).await;
                    log::trace!("UDP receiver thread got recv_many");
                    for msg in recv_msgs.drain(..) {
                        match self.iface.send(msg) {
                            Ok(()) => {
                                log::debug!("UDP receiver thread sent packet successfully");
                            },
                            Err(e) => {
                                log::debug!("Error processing packet: {e}");
                            }
                        }
                    }
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
            Type::DGRAM.nonblocking(),
            Some(Protocol::UDP),
        )?;
        udp.set_reuse_address(true)?;
        let udp = UdpSocket::from_std(udp.into())?;
        let real_addr = udp.local_addr()?;
        let (mut iface, iface_pvt) = iface::new("UDPAddrIface");
        let (tgo, tgo_r) =
            tokio::sync::mpsc::channel(TO_GO_OUT_QUEUE);
        let (incoming, incoming_r) =
            tokio::sync::mpsc::channel(INCOMING_QUEUE);
        let internal = Arc::new(UDPAddrIfaceInternal {
            iface: iface_pvt,
            udp,
            to_go_out_recv: Mutex::new(tgo_r),
            to_go_out_send: tgo,
            incoming_recv: Mutex::new(incoming_r),
            incoming_send: incoming,
        });
        iface.set_receiver(Arc::clone(&internal));

        let workers = WORKERS;
        let workers = if workers < 2 {
            log::warn!("UDPAddrIface WORKERS = {workers} is too few, using 2");
            2
        } else {
            workers
        };
        for _ in 0..workers {
            tokio::task::spawn(Arc::clone(&internal).recv_worker());
            tokio::task::spawn(Arc::clone(&internal).send_worker());
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
}