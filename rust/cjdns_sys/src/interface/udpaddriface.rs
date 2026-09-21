use num_enum::{IntoPrimitive, TryFromPrimitive};
use socket2::{Protocol, SockAddr, Type};
use tokio::net::UdpSocket;
use tokio::sync::mpsc::{Receiver, Sender};
use tokio::sync::{Mutex,RwLock};
use tokio::task::JoinSet;
use crate::util::now_ms;
use crate::util::sockaddr::Sockaddr;
use std::collections::HashMap;
use std::convert::TryFrom;
use std::io::ErrorKind;
use std::sync::atomic::Ordering::Relaxed;
use std::sync::atomic::{AtomicI32, AtomicU32};
use std::sync::Arc;
use crate::interface::wire::message::Message;
use crate::external::interface::iface::{self, IfRecv, Iface, IfacePvt};
use eyre::{Context, Result, eyre};
use std::net::SocketAddr;

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

struct UdpConn {
    sock: Option<UdpSocket>,
    last_recv_sec: AtomicU32,
}
impl UdpConn {
    fn new(sock: Option<UdpSocket>) -> Arc<Self> {
        Arc::new(Self {
            sock,
            last_recv_sec: AtomicU32::new((now_ms() / 1000) as u32),
        })
    }
    fn notify_received(&self) {
        self.last_recv_sec.store((now_ms() / 1000) as u32, Relaxed)
    }
    fn last_recv_sec(&self) -> u32 {
        self.last_recv_sec.load(Relaxed)
    }
    async fn readable(self: Arc<Self>) -> (Option<Arc<Self>>, Result<()>) {
        match &self.sock {
            Some(sock) => {
                if let Err(e) = sock.readable().await {
                    (Some(self), Err(e.into()))
                } else {
                    (Some(self), Ok(()))
                }
            }
            None => (Some(self), Err(eyre!("readable called on main socket"))),
        }
    }
}

fn create_socket(sa: &SockAddr) -> Result<UdpSocket> {
    let udp = socket2::Socket::new(
        sa.domain(),
        Type::DGRAM,
        Some(Protocol::UDP),
    )?;
    udp.set_nonblocking(true)?;
    udp.set_reuse_address(true)?;
    udp.bind(sa)?;
    let udp = UdpSocket::from_std(udp.into())?;
    Ok(udp)
}

fn set_dscp(sock: &UdpSocket, dscp: u8) -> Result<()> {
    let local_addr = sock.local_addr()?;
    let tos = (dscp as u32) << 2;
    if local_addr.is_ipv6() {
        socket2::SockRef::from(&sock).set_tclass_v6(tos)?;
    } else {
        sock.set_tos(tos)?;
    }
    Ok(())
}

struct UDPAddrIfaceInternal {
    iface: IfacePvt,
    mainsock: UdpSocket,
    destmap: RwLock<HashMap<SocketAddr,Arc<UdpConn>>>,
    destmap_ver: AtomicU32,
    conn_timeout_secs: u32,
    to_go_out_recv: Mutex<Receiver<(Message,SocketAddr)>>,
    to_go_out_send: Sender<(Message,SocketAddr)>,
    randomize_source_port: bool,
    proto_sa: SockAddr,
    dscp: AtomicI32,

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
        loop {
            self.send_worker_set_state(n, SendWorkerState::WaitLock);
            let mut tgo = self.to_go_out_recv.lock().await;
            self.send_worker_set_state(n, SendWorkerState::RecvBatch);
            let msg = tgo.recv().await;
            drop(tgo);
            self.send_worker_set_state(n, SendWorkerState::SendBatch);

            let (msg, sa) = if let Some(sm) = msg {
                sm
            } else {
                continue;
            };

            let conn = if self.randomize_source_port {
                let mrs = self.min_last_recv_sec();
                let destmap = self.destmap.read().await;
                let conn = if let Some(conn) = destmap.get(&sa) {
                    if conn.last_recv_sec() > mrs {
                        Some(Arc::clone(conn))
                    } else {
                        None
                    }
                } else {
                    None
                };
                drop(destmap);
                if conn.is_some() {
                    conn
                } else {
                    let sock = match create_socket(&self.proto_sa) {
                        Ok(sock) => sock,
                        Err(e) => {
                            log::warn!("DROP Error creating socket to send packet to {sa}: {e}");
                            continue;
                        }
                    };
                    let dscp = self.dscp.load(Relaxed);
                    if dscp >= 0 {
                        if let Err(e) = set_dscp(&sock, dscp as u8) {
                            log::warn!("Unable to set DSCP on new socket {sa}: {e}");
                        }
                    }
                    let mut destmap = self.destmap.write().await;
                    destmap.retain(|_,s|s.last_recv_sec() > mrs);
                    self.destmap_ver.fetch_add(1, Relaxed);
                    if let Some(s) = destmap.get(&sa) {
                        let conn = Arc::clone(s);
                        Some(conn)
                    } else {
                        let conn = UdpConn::new(Some(sock));
                        destmap.insert(sa.clone(), Arc::clone(&conn));
                        Some(conn)
                    }
                }
            } else {
                None
            };

            // println!("got message with length: {}", msg.len());
            let bytes = msg.bytes();
            match if let Some(conn) = &conn {
                if let Some(sock) = &conn.sock {
                    sock
                } else {
                    &self.mainsock
                }
            } else {
                &self.mainsock
            }.send_to(bytes, &sa).await {
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
    fn recv_worker_set_state(self: &Arc<Self>, n: usize, state: RecvWorkerState) {
        self.recv_worker_states[n].store(state as i32, std::sync::atomic::Ordering::Relaxed);
    }
    fn min_last_recv_sec(self: &Arc<Self>) -> u32 {
        (now_ms() / 1000) as u32 - self.conn_timeout_secs
    }
    async fn mainsock_readable(self: Arc<Self>) -> (Option<Arc<UdpConn>>, Result<()>) {
        if let Err(e) = self.mainsock.readable().await {
            (None, Err(e.into()))
        } else {
            (None, Ok(()))
        }
    }
    async fn notify_received_main(self: &Arc<Self>, sa: &SocketAddr) {
        if !self.randomize_source_port {
            return;
        }
        let destmap = self.destmap.read().await;
        if let Some(x) = destmap.get(sa) {
            if x.sock.is_none() {
                x.notify_received();
                return;
            }
        }
        drop(destmap);
        let mrs = self.min_last_recv_sec();
        let mut destmap = self.destmap.write().await;
        destmap.retain(|_,s|s.last_recv_sec() > mrs);
        destmap.insert(sa.clone(), UdpConn::new(None));
        self.destmap_ver.fetch_add(1, Relaxed);
    }
    async fn recv_worker(self: Arc<Self>, n: usize) {
        let mut message = None;
        let mut conns = Vec::new();
        let mut destmap_ver = 0;
        let mut js = JoinSet::new();
        let mut js_need_update = true;
        loop {
            // Allocate a Message
            let mut msg = if let Some(msg) = message.take() {
                msg
            } else {
                let mut msg = Message::new(PADDING_AMOUNT + BUFFER_CAP);
                msg.allocate_uninitialized(BUFFER_CAP).unwrap();
                msg
            };

            {
                let dv = self.destmap_ver.load(Relaxed);
                if destmap_ver < dv {
                    conns.clear();
                    let l = self.destmap.read().await;
                    for conn in l.values().cloned() {
                        if conn.sock.is_some() {
                            conns.push(conn);
                        }
                    }
                    drop(l);
                    destmap_ver = dv;
                    js_need_update = true;
                }
            }
            if js_need_update {
                js.abort_all();
                while let Some(_) = js.join_next().await {}
                for conn in conns.iter().cloned() {
                    js.spawn(conn.readable());
                }
                js.spawn(Arc::clone(&self).mainsock_readable());
            }

            self.recv_worker_set_state(n, RecvWorkerState::RecvBatch);

            let mut result = None;
            while let Some(res) = js.join_next().await {
                let (conn, rres) = match res {
                    Ok(res) => res,
                    Err(e) => {
                        log::warn!("JoinError from recv worker {e}");
                        js_need_update = true;
                        continue;
                    }
                };
                if let Err(e) = rres {
                    log::warn!("Error calling socket.ready(): {e}");
                }

                let x = if let Some(x) =
                    if let Some(conn) = conn {
                        if let Some(sock) = &conn.sock {
                            let x = sock.try_recv_from(msg.bytes_mut());
                            if x.is_ok() {
                                conn.notify_received();
                            }
                            js.spawn(conn.readable());
                            Some(x)
                        } else {
                            None
                        }
                    } else {
                        None
                    }
                {
                    x
                } else {
                    let x = self.mainsock.try_recv_from(msg.bytes_mut());
                    if let Ok((_, sa)) = &x {
                        self.notify_received_main(sa).await;
                    }
                    js.spawn(Arc::clone(&self).mainsock_readable());
                    x
                };

                match x {
                    Ok(res) => {
                        result = Some(res);
                        break;
                    }
                    Err(e) => {
                        if e.kind() == ErrorKind::WouldBlock {
                            // Most likely another thread got it first
                            continue;
                        }
                        log::warn!("Error receiving UDP message {e}");
                        continue;
                    }
                }
            }
            self.recv_worker_set_state(n, RecvWorkerState::RecievedBatch);

            if let Some((byte_count,from)) = result {
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
            } else {
                message = Some(msg);
            }
        }
    }
}


pub struct UDPAddrIface {
    internal: Arc<UDPAddrIfaceInternal>,
    pub local_addr: SocketAddr,
}

impl UDPAddrIface {
    pub fn new(
        bind_addr: &SocketAddr,
        conn_timeout_secs: u32,
    ) -> Result<(Self,Iface)> {
        let sa = SockAddr::from((*bind_addr).clone());
        let udp = create_socket(&sa)?;
        let real_addr = udp.local_addr()?;
        let (mut iface, iface_pvt) = iface::new("UDPAddrIface");
        let (tgo, tgo_r) =
            tokio::sync::mpsc::channel(TO_GO_OUT_QUEUE);

        let proto_sa = SockAddr::from(SocketAddr::new(
            if sa.domain() == socket2::Domain::IPV4 {
                std::net::Ipv4Addr::UNSPECIFIED.into()
            } else {
                std::net::Ipv6Addr::UNSPECIFIED.into()
            },
            0,
        ));

        let workers = num_cpus::get();
        let workers = if workers < 2 {
            log::warn!("UDPAddrIface WORKERS = {workers} is too few, using 2");
            2
        } else {
            workers
        };

        let internal = Arc::new(UDPAddrIfaceInternal {
            iface: iface_pvt,
            mainsock: udp,
            destmap: Default::default(),
            destmap_ver: AtomicU32::new(0),
            proto_sa,
            randomize_source_port: conn_timeout_secs != 0,
            conn_timeout_secs,
            dscp: AtomicI32::new(-1),

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
        set_dscp(&self.internal.mainsock, dscp)?;
        self.internal.dscp.store(dscp as i32, Relaxed);
        Ok(())
    }

    pub fn set_broadcast(&self, enable: bool) -> Result<()> {
        self.internal.mainsock.set_broadcast(enable)?;
        Ok(())
    }

    #[cfg(windows)]
    pub fn get_fd(&self) -> u32 {
        -1
    }

    #[cfg(not(windows))]
    pub fn get_fd(&self) -> i32 {
        use std::os::fd::AsFd;
        use std::os::fd::AsRawFd;
        let bfd = self.internal.mainsock.as_fd();
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
