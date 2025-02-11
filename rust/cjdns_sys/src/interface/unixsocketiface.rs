use tokio::net::UnixListener;
use parking_lot::RwLock;
use crate::util::callable::Callable;
use crate::util::sockaddr::Sockaddr;
use std::collections::HashMap;
use std::convert::TryFrom;
use std::os::unix::net::UnixStream;
use std::sync::atomic::AtomicU32;
use std::sync::{Arc, Weak};
use crate::interface::wire::message::Message;
use crate::external::interface::iface::{IfRecv, Iface, IfacePvt};
use eyre::{eyre, Result};
use super::socketiface::{SocketIface, SocketType};

struct ClientRecv {
    handle: Sockaddr,
    addr_iface: Arc<IfacePvt>,
    ussm: Weak<RwLock<UnixSocketServerMut>>,
    client_n: u32,
}
/// Messages coming FROM the client
impl IfRecv for ClientRecv {
    fn recv(&self, mut m: Message) -> Result<()> {
        if m.len() == 0 {
            if let Some(ussm) = self.ussm.upgrade() {
                ussm.write().clients.remove(&self.client_n);
            }
            Ok(())
        } else {
            // TODO: If the message is 0 length, drop the client
            m.push_bytes(self.handle.bytes())?;
            self.addr_iface.send(m)
        }
    }
}

struct Client {
    si: SocketIface,
    iface: Iface,
    ifacep: Arc<IfacePvt>,
}
impl Drop for Client {
    fn drop(&mut self) {
        println!("Client was dropped");
    }
}

#[derive(Default)]
struct UnixSocketServerMut {
    clients: HashMap<u32,Client>,
}
impl IfRecv for Arc<RwLock<UnixSocketServerMut>> {
    fn recv(&self, mut m: Message) -> Result<()> {
        let sa = Sockaddr::try_from(m.bytes())?;
        let handle = sa.as_handle().ok_or_else(||eyre!("Sockaddr not a handle type"))?;
        m.discard_bytes(sa.byte_len())?;
        let l = self.read();
        let cli = l.clients.get(&handle)
            .ok_or_else(||eyre::eyre!("No client found for handle: {handle}"))?;
        let iface = Arc::clone(&cli.ifacep);
        drop(l);
        iface.send(m)
    }
}

pub struct UnixSocketServerInner {
    addr_iface: Arc<IfacePvt>,
    inner: Arc<RwLock<UnixSocketServerMut>>,
    next_client_n: AtomicU32,
    ul: UnixListener,
    on_new_client: tokio::sync::Mutex<Option<Arc<dyn Callable<Sockaddr,()>>>>,
    done_r: tokio::sync::broadcast::Receiver<()>,
    name: Arc<String>,
}
impl UnixSocketServerInner {
    async fn accept(self: &Arc<Self>) -> Result<()> {
        let (us, sa) = self.ul.accept().await?;
        log::info!("Got incoming Unix Socket connection from {sa:?}");
        let us = us.into_std()?;
        us.set_nonblocking(true)?;
        let mut si = SocketIface::new(vec![ us ], SocketType::Stream)?;

        let client_n = self.next_client_n.fetch_add(1, std::sync::atomic::Ordering::Relaxed);
        let handle = Sockaddr::from(client_n);

        let (mut iface, ifacep) = Iface::new(format!("UnixSocketIface({sa:?} / {client_n})"));
        iface.set_receiver(ClientRecv{
            handle,
            addr_iface: Arc::clone(&self.addr_iface),
            ussm: Arc::downgrade(&self.inner),
            client_n 
        });
        iface.plumb(&mut si.iface)?;
        let cli = Client{
            si,
            iface,
            ifacep: Arc::new(ifacep),
        };
        self.inner.write().clients.insert(client_n, cli);
        if let Some(onc) = self.on_new_client.lock().await.clone() {
            onc.call(Sockaddr::from(client_n));
        }
        Ok(())
    }
    async fn listen(self: Arc<Self>) {
        let mut done_r = self.done_r.resubscribe();
        loop {
            tokio::select! {
                r = self.accept() => {
                    match r {
                        Ok(()) => {},
                        Err(e) => {
                            log::warn!("Error accepting from socket: {e}");
                            continue;
                        }
                    }
                }
                _ = done_r.recv() => {
                    log::info!("Got shutdown for {}", self.name);
                    break;
                }
            }
        }
    }
}

pub struct UnixSocketServer {
    pub iface: Iface,
    inner: Arc<UnixSocketServerInner>,
    _done: tokio::sync::broadcast::Sender<()>,
}
impl UnixSocketServer {
    pub fn new(path: &str) -> Result<Self> {
        let ul: UnixListener = UnixListener::bind(path)?;
        let (mut iface, ifacep) = Iface::new(format!("UnixSocketServer({path})"));
        let (_done, done_r) = tokio::sync::broadcast::channel(1);
        let inner = Arc::new(UnixSocketServerInner{
            addr_iface: Arc::new(ifacep),
            inner: Default::default(),
            next_client_n: Default::default(),
            ul,
            on_new_client: Default::default(),
            done_r,
            name: Arc::clone(&iface.name),
        });
        iface.set_receiver(Arc::clone(&inner.inner));
        tokio::task::spawn(Arc::clone(&inner).listen());
        Ok(Self { iface, inner, _done })
    }

    pub async fn on_new_client(
        &self,
        cli: Option<Arc<dyn Callable<Sockaddr,()>>>,
    ) -> Option<Arc<dyn Callable<Sockaddr,()>>> {
        if let Some(cli) = cli {
            self.inner.on_new_client.lock().await.replace(cli)
        } else {
            self.inner.on_new_client.lock().await.take()
        }
    }
}

pub struct UnixSocketClient {
    si: SocketIface,
}
impl UnixSocketClient {
    pub fn iface(&mut self) -> &mut Iface {
        &mut self.si.iface
    }
    pub fn new(path: &str) -> Result<Self> {
        let us = UnixStream::connect(path)?;
        us.set_nonblocking(true)?;
        let si = SocketIface::new(vec![ us ], SocketType::Stream)?;
        Ok(Self { si })
    }
}