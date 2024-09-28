use std::collections::VecDeque;
use std::os::fd::AsRawFd;
use std::sync::atomic::AtomicI32;
use std::time::Duration;
use libc::cmsghdr;
use num_enum::{IntoPrimitive, TryFromPrimitive};
use tokio::io::unix::AsyncFd;
use tokio::sync::mpsc::{Receiver, Sender};
use tokio::sync::Mutex;
use crate::rtypes::RTypes_SocketType;
use crate::util::sockaddr::Sockaddr;
use std::convert::TryFrom;
use std::sync::Arc;
use crate::interface::wire::message::Message;
use crate::external::interface::iface::{self, IfRecv, Iface, IfacePvt};
use anyhow::{Context, Result};

const RECV_BATCH: usize = 8;
const SEND_BATCH: usize = 8;

const TO_GO_OUT_QUEUE: usize = 64;

const BUFFER_CAP: usize = 3496;
const PADDING_AMOUNT: usize = 512;

// sendmmsg() is only available on Linux, so on other systems we'll just emulate it.
#[repr(C)]
struct Mmsghdr {
    msg_hdr: libc::msghdr,
    msg_len: libc::c_uint,
}

fn recvmmsg(
    sockfd: libc::c_int,
    msgvec: &mut [Mmsghdr],
    flags: libc::c_int,
) -> Result<(), std::io::Error> {
    for msg in msgvec {
        let ret = unsafe { libc::recvmsg(sockfd, &mut msg.msg_hdr as *mut _, flags) };
        if ret < 0 {
            return Err(std::io::Error::last_os_error());
        }
        msg.msg_len = ret as libc::c_uint;
    }
    Ok(())
}

fn read(
    sockfd: libc::c_int,
    buf: &mut [u8],
) -> Result<usize, std::io::Error> {
    let ret = unsafe { libc::read(sockfd, buf.as_mut_ptr() as _, buf.len()) };
    if ret < 0 {
        return Err(std::io::Error::last_os_error());
    }
    Ok(ret as usize)
}

fn write(
    sockfd: libc::c_int,
    buf: &[u8],
) -> Result<usize, std::io::Error> {
    let ret = unsafe { libc::write(sockfd, buf.as_ptr() as _, buf.len()) };
    if ret < 0 {
        return Err(std::io::Error::last_os_error());
    }
    Ok(ret as usize)
}

fn sendmmsg(
    sockfd: libc::c_int,
    msgvec: &mut [Mmsghdr],
    flags: libc::c_int,
) -> Result<(), std::io::Error> {
    for msg in msgvec {
        let ret = unsafe { libc::sendmsg(sockfd, &msg.msg_hdr as *const _, flags) };
        if ret < 0 {
            return Err(std::io::Error::last_os_error());
        }
        msg.msg_len = ret as libc::c_uint;
    }
    Ok(())
}

struct Additional {
    anciliary: [u8; 64],
    address: [u8; 128],
}

pub type SocketType = RTypes_SocketType;

struct IoContext<const COUNT: usize> {
    hdrs: [Mmsghdr; COUNT],
    iovecs: [libc::iovec; COUNT],
    add: [Additional; COUNT],
    sockfd: libc::c_int,
    st: SocketType,
}
unsafe impl<const COUNT: usize> Send for IoContext<COUNT> {}

impl <const COUNT: usize> IoContext<COUNT> {
    fn new(sockfd: libc::c_int, st: SocketType) -> Self {
        Self{
            hdrs: unsafe { std::mem::zeroed() },
            iovecs: unsafe { std::mem::zeroed() },
            add: unsafe { std::mem::zeroed() },
            sockfd,
            st,
        }
    }
    /// send_stream sends a single message with all of the Message contents
    /// packet into a single array of iovec.
    /// With the exception that if you send_stream with a Message containing
    /// a file descriptor, that Message will be sent separately.
    /// send_stream always sends a single message (sendmmsg of 1)
    /// Do not call this on a non-stream socket.
    fn send_stream(&mut self, messages: &mut VecDeque<Message>) -> Option<std::io::Error> {
        let mut i = 0;
        let hdr = &mut self.hdrs[0];
        let add = &mut self.add[0];
        let mut send_fd = -1;
        let mut total_len = 0;
        for (msg, iovec) in messages.iter_mut().zip(self.iovecs.iter_mut()) {
            let fd = msg.get_fd();
            if fd != -1 && i > 0 {
                break;
            }
            iovec.iov_base = msg.bytes_mut().as_mut_ptr() as _;
            iovec.iov_len = msg.len();
            total_len += iovec.iov_len;
            i += 1;
            if fd != -1 {
                send_fd = fd;
                break;
            }
        }
        hdr.msg_hdr.msg_iov = self.iovecs.as_mut_ptr();
        hdr.msg_hdr.msg_iovlen = i;

        if send_fd != -1 {
            hdr.msg_hdr.msg_control = add.anciliary.as_mut_ptr() as _;
            hdr.msg_hdr.msg_controllen = add.anciliary.len() as _;
            let bottom = add.anciliary.as_ptr() as usize;
            let top = bottom + add.anciliary.len();
            unsafe {
                let cmsg = libc::CMSG_FIRSTHDR(&hdr.msg_hdr as *const _);
                assert!((cmsg as usize) >= bottom && (cmsg as usize) < top);
                let d = libc::CMSG_DATA(cmsg) as *mut i32;
                assert!((d as usize) >= bottom && (d as usize + std::mem::size_of::<i32>()) <= top);
                *d = send_fd;
                let len = libc::CMSG_LEN(std::mem::size_of::<i32>() as _);
                (*cmsg).cmsg_len = len as _;
                (*cmsg).cmsg_type = libc::SCM_RIGHTS;
                (*cmsg).cmsg_level = libc::SOL_SOCKET;
                hdr.msg_hdr.msg_controllen = len as _;
            }
        } else {
            hdr.msg_hdr.msg_control = std::ptr::null_mut();
            hdr.msg_hdr.msg_controllen = 0;
        }

        hdr.msg_hdr.msg_name = std::ptr::null_mut();
        hdr.msg_hdr.msg_namelen = 0;

        hdr.msg_len = total_len as _;
        hdr.msg_hdr.msg_flags = 0;

        let ret = sendmmsg(self.sockfd, &mut self.hdrs[0..1], libc::MSG_DONTWAIT);

        // Now we need to go through our Messages and figure out which ones are already sent
        // which ones could not be sent, and which one was partially sent (if any).
        // Fully sent -> msg.clear()
        // Partially sent -> msg.discard_bytes() so the next send will pick up where it left off
        // Not sent -> leave it alone
        let mut sent = self.hdrs[0].msg_len as _;
        for msg in messages.iter_mut() {
            if sent >= msg.len() {
                sent -= msg.len();
                msg.clear();
            } else if sent == 0_usize {
                break;
            } else {
                msg.discard_bytes(sent).unwrap();
                break;
            }
        }

        ret.err()
    }

    /// Send frame-like messages or udp packets
    fn send_frames(&mut self, messages: &mut VecDeque<Message>) -> Option<std::io::Error> {
        let mut i = 0;
        for (((msg, hdr), add), iovec) in
            messages.iter_mut().zip(self.hdrs.iter_mut()).zip(self.add.iter_mut()).zip(self.iovecs.iter_mut())
        {
            if self.st == SocketType::SendToFrames {
                let sa = match Sockaddr::try_from(msg.bytes()) {
                    Ok(sa) => sa,
                    Err(e) => {
                        log::info!("DROP: Socket message unparsable sockaddr: {e}");
                        msg.clear();
                        continue;
                    }
                };
                msg.discard_bytes(sa.byte_len()).expect("Message too short for sockaddr");
                add.address.copy_from_slice(sa.bytes());

                hdr.msg_hdr.msg_name = add.address.as_mut_ptr() as _;
                hdr.msg_hdr.msg_namelen = sa.byte_len() as _;
            } else {
                hdr.msg_hdr.msg_name = std::ptr::null_mut();
                hdr.msg_hdr.msg_namelen = 0;
            }

            iovec.iov_base = msg.bytes_mut() as *mut _ as *mut _;
            iovec.iov_len = msg.len();
            hdr.msg_hdr.msg_iov = iovec as _;
            hdr.msg_hdr.msg_iovlen = 1;

            // FDs can only be reliably sent over stream unix sockets so we can skip.
            hdr.msg_hdr.msg_control = std::ptr::null_mut();
            hdr.msg_hdr.msg_controllen = 0;
            if msg.get_fd() != -1 {
                log::warn!("Attempted to send an FD over a non-stream socket, ignored");
            }

            hdr.msg_hdr.msg_flags = 0;
            hdr.msg_len = iovec.iov_len as _;

            i += 1;
        }

        let ret = sendmmsg(self.sockfd, &mut self.hdrs[0..i], 0);

        for (msg, hdr) in messages.iter_mut().zip(self.hdrs.iter_mut()) {
            let sent = hdr.msg_len as usize;
            if sent == msg.len() {
                msg.clear();
            } else if sent == 0_usize {
                break;
            } else {
                log::warn!("sendmmsg() claims [{sent}] bytes were sent on datagram message with size [{}]", msg.len());
                // We're going to drop this message because the following message may have sent properly
                // And by dropping it, we can offer the caller a simple promise that if msg N was not sent,
                // msg N+1 was not sent either.
                msg.clear();
            }
        }

        ret.err()
    }

    fn write_frames(&mut self, messages: &mut VecDeque<Message>) -> Option<std::io::Error> {
        for m in messages.iter_mut() {
            match write(self.sockfd, m.bytes()) {
                Ok(l) if l == m.len() => {
                    m.clear();
                },
                Ok(l) => {
                    log::warn!("write_frames: message truncated from {} to {}",
                        m.len(), l);
                    m.clear();
                    return None;
                }
                Err(e) => {
                    return Some(e);
                }
            }
        }
        None
    }

    fn send(&mut self, messages: &mut VecDeque<Message>) -> Option<std::io::Error> {
        if self.st == SocketType::Stream {
            self.send_stream(messages)
        } else if self.st == SocketType::ReadFrames {
            self.write_frames(messages)
        } else {
            self.send_frames(messages)
        }
    }

    fn read(&mut self, messages: &mut VecDeque<Message>) -> (usize, Option<std::io::Error>) {
        let mut i = 0;
        for msg in messages.iter_mut() {
            match read(self.sockfd, msg.bytes_mut()) {
                Ok(c) => {
                    msg.set_len(c).unwrap();
                    i += 1;
                }
                Err(e) => {
                    return (i, Some(e));
                }
            }
        }
        (i, None)
    }

    /// Receive either stream bytes OR frames
    /// Return value is the number of messages received plus the error which occurred that stopped further receiving.
    fn recv(&mut self, messages: &mut VecDeque<Message>) -> (usize, Option<std::io::Error>) {
        let mut i = 0;
        for (((msg, hdr), add), iovec) in
            messages.iter_mut().zip(self.hdrs.iter_mut()).zip(self.add.iter_mut()).zip(self.iovecs.iter_mut())
        {
            iovec.iov_base = msg.bytes_mut() as *mut _ as *mut _;
            iovec.iov_len = msg.len();
            hdr.msg_hdr.msg_iov = iovec as _;
            hdr.msg_hdr.msg_iovlen = 1;

            hdr.msg_hdr.msg_control = add.anciliary.as_mut_ptr() as _;
            hdr.msg_hdr.msg_controllen = add.anciliary.len() as _;

            if self.st == SocketType::SendToFrames {
                hdr.msg_hdr.msg_name = add.address.as_mut_ptr() as *mut _;
                hdr.msg_hdr.msg_namelen = add.address.len() as _;
            } else {
                hdr.msg_hdr.msg_name = std::ptr::null_mut();
                hdr.msg_hdr.msg_namelen = 0;
            }

            hdr.msg_len = !0;
            
            // On Linux, msg_flags is ignored.
            hdr.msg_hdr.msg_flags = 0;

            i += 1;
        }

        let res = recvmmsg(self.sockfd, &mut self.hdrs[0..i], libc::MSG_DONTWAIT);

        i = 0;
        for ((msg, hdr), add) in
            messages.iter_mut().zip(self.hdrs.iter_mut()).zip(self.add.iter_mut())
        {
            if hdr.msg_len == !0 {
                break;
            }
            log::trace!("Received message");
            i += 1;
            msg.set_len(hdr.msg_len as _).unwrap();
            if self.st == SocketType::SendToFrames {
                if hdr.msg_hdr.msg_namelen == 0_u32 {
                    log::info!("DROP: Socket message with 0 name length and push_from_addr");
                    msg.clear();
                    continue;
                }
                let sa = match Sockaddr::try_from(&add.address[0..hdr.msg_hdr.msg_namelen as _]) {
                    Ok(sa) => sa,
                    Err(e) => {
                        log::info!("DROP: Socket message unparsable sockaddr: {e}");
                        msg.clear();
                        continue;
                    }
                };
                msg.push_bytes(sa.bytes()).unwrap();
            }
            if hdr.msg_hdr.msg_controllen > 0 {
                // log::debug!("Message has control data len:{} hex:{}",
                //     hdr.msg_hdr.msg_controllen, hex::encode(&add.anciliary[..]));
                let bottom = add.anciliary.as_ptr() as usize;
                let top = bottom + add.anciliary.len();
                let mut cmsg = unsafe { libc::CMSG_FIRSTHDR(&hdr.msg_hdr as *const _) };
                if cmsg.is_null() {
                    log::debug!("Message CMSG header is null");
                }
                while !cmsg.is_null() {
                    assert!((cmsg as usize) >= bottom && (cmsg as usize) < top);
                    let c = unsafe { *cmsg };
                    let data_len = c.cmsg_len as usize - std::mem::size_of::<cmsghdr>();
                    if c.cmsg_type != libc::SCM_RIGHTS {
                        log::debug!("Unexpected cmsg type in socket message: {}", c.cmsg_type);
                    } else if data_len != 4 {
                        let buf = unsafe {
                            let s = std::slice::from_raw_parts(libc::CMSG_DATA(cmsg), data_len);
                            hex::encode(s)
                        };
                        log::debug!("Passing file descriptor cmsg length unexpected: {} hex:{}", data_len, buf);
                    } else {
                        let fd = unsafe { *(libc::CMSG_DATA(cmsg) as *mut i32) };
                        msg.set_fd(fd);
                        log::debug!("Got file descriptor {fd} from message");
                        break;
                    }
                    cmsg = unsafe { libc::CMSG_NXTHDR(&hdr.msg_hdr as *const _, cmsg) };
                }
            }
        }

        (i, res.err())
    }
}

#[derive(Debug,IntoPrimitive,TryFromPrimitive)]
#[repr(i32)]
pub enum SendWorkerState {
    Invalid = -1,
    Initializing = 0,
    WaitLock = 1,
    RecvBatch = 2,
    WaitFdWritable = 3,
    WaitFdError = 4,
    SendBatch = 5,
    SentBatch = 6,
}

#[derive(Debug,IntoPrimitive,TryFromPrimitive)]
#[repr(i32)]
pub enum RecvWorkerState {
    Invalid = -1,
    Initializing = 0,
    WaitFdReadable = 1,
    WaitFdError = 2,
    RecvBatch = 3,
    RecievedBatch = 4,
    IfaceSend = 5,
    Yield = 6,
}

struct SocketIfaceInternal<T: AsRawFd + Sync + Send> {
    iface: IfacePvt,
    st: SocketType,
    afds: Vec<AsyncFd<T>>,

    to_go_out_recv: Mutex<Receiver<Message>>,
    to_go_out_send: Sender<Message>,
    done_r: tokio::sync::broadcast::Receiver<()>,

    send_worker_states: Vec<AtomicI32>,
    recv_worker_states: Vec<AtomicI32>,
}
impl<T: AsRawFd + Sync + Send> IfRecv for Arc<SocketIfaceInternal<T>> {
    fn recv(&self, m: Message) -> Result<()> {
        self.to_go_out_send.try_send(m).context("Not enough buffer space to send pkt")?;
        Ok(())
    }
}

impl<T: AsRawFd + Sync + Send + 'static> SocketIfaceInternal<T> {
    async fn worker(self: Arc<Self>, n: usize, send: bool) {
        let mut done = self.done_r.resubscribe();
        if send {
            tokio::select! {
                _ = self.send_worker(n) => {},
                _ = done.recv() => {
                    log::info!("Send worker [{n}] received done");
                },
            }
        } else {
            tokio::select! {
                _ = self.recv_worker(n) => {},
                _ = done.recv() => {
                    log::info!("Recv worker [{n}] received done");
                },
            }
        }
    }
    fn send_worker_set_state(self: &Arc<Self>, n: usize, state: SendWorkerState) {
        self.send_worker_states[n].store(state as i32, std::sync::atomic::Ordering::Relaxed);
    }
    async fn send_worker(self: Arc<Self>, n: usize) {
        let fd_num = n % self.afds.len();
        let mut ctx: IoContext<SEND_BATCH> = IoContext::new(self.afds[fd_num].as_raw_fd(), self.st);
        let mut batch = VecDeque::with_capacity(SEND_BATCH);
        let mut batch_vec = Vec::with_capacity(SEND_BATCH);

        loop {
            self.send_worker_set_state(n, SendWorkerState::WaitLock);
            let mut tgo = self.to_go_out_recv.lock().await;
            self.send_worker_set_state(n, SendWorkerState::RecvBatch);
            tgo.recv_many(&mut batch_vec, RECV_BATCH - batch.len()).await;
            drop(tgo);
            batch.extend(batch_vec.drain(..));

            self.send_worker_set_state(n, SendWorkerState::WaitFdWritable);
            let mut writable = match self.afds[fd_num].writable().await {
                Ok(r) => r,
                Err(e) => {
                    self.send_worker_set_state(n, SendWorkerState::WaitFdError);
                    log::info!("Error polling fd.writable(): {e} - sleep 1 second");
                    tokio::time::sleep(Duration::from_secs(1)).await;
                    continue;
                }
            };

            self.send_worker_set_state(n, SendWorkerState::SendBatch);
            let err = ctx.send(&mut batch);
            self.send_worker_set_state(n, SendWorkerState::SentBatch);

            // If err is EAGAIN / EWOULDBLOCK then we clear the readable state
            // If received is more than zero, we pop and forward those messages
            // If err is EINTER then we ignore and repeat
            // If err is something else, we warn about it.
            if let Some(err) = err {
                match err.kind() {
                    std::io::ErrorKind::WouldBlock => writable.clear_ready(),
                    std::io::ErrorKind::Interrupted => {},
                    _ => {
                        log::info!("Error writing to socket: {err}");
                        writable.clear_ready();
                    }
                }
            }

            // Go through the messages and pop / discard all empty messages
            let mut successfully_sent = 0;
            while let Some(msg) = batch.pop_front() {
                if msg.len() > 0 {
                    batch.push_front(msg);
                    break;
                }
                successfully_sent += 1;
            }
            if successfully_sent == 0 {
                log::debug!("Worker {n} cycled with no messages sent");
            }
        }
    }
    fn recv_worker_set_state(self: &Arc<Self>, n: usize, state: RecvWorkerState) {
        self.recv_worker_states[n].store(state as i32, std::sync::atomic::Ordering::Relaxed);
    }
    async fn recv_worker(self: Arc<Self>, n: usize) {
        let fd_num = n % self.afds.len();
        let mut ctx: IoContext<RECV_BATCH> = IoContext::new(self.afds[fd_num].as_raw_fd(), self.st);
        let mut batch = VecDeque::with_capacity(RECV_BATCH);
        loop {
            while batch.len() < RECV_BATCH {
                let mut msg = Message::new(PADDING_AMOUNT + BUFFER_CAP);
                msg.allocate_uninitialized(BUFFER_CAP).unwrap();
                batch.push_back(msg);
            }
            self.recv_worker_set_state(n, RecvWorkerState::WaitFdReadable);
            let mut readable = match self.afds[fd_num].readable().await {
                Ok(r) => r,
                Err(e) => {
                    self.recv_worker_set_state(n, RecvWorkerState::WaitFdError);
                    log::info!("Error polling fd.readable(): {e} - sleep 1 second");
                    tokio::time::sleep(Duration::from_secs(1)).await;
                    continue;
                }
            };
            self.recv_worker_set_state(n, RecvWorkerState::RecvBatch);
            let (received, err) = if self.st == SocketType::ReadFrames {
                ctx.read(&mut batch)
            } else {
                ctx.recv(&mut batch)
            };
            self.recv_worker_set_state(n, RecvWorkerState::RecievedBatch);
            // If err is EAGAIN / EWOULDBLOCK then we clear the readable state
            // If received is more than zero, we pop and forward those messages
            // If err is EINTER then we ignore and repeat
            // If err is something else, we warn about it.
            if let Some(err) = err {
                match err.kind() {
                    std::io::ErrorKind::WouldBlock => readable.clear_ready(),
                    std::io::ErrorKind::Interrupted => {},
                    _ => {
                        log::info!("Error reading from socket: {err}");
                        readable.clear_ready();
                    }
                }
            }

            for _ in 0..received {
                if let Some(mut msg) = batch.pop_front() {
                    if msg.cap() == 0 {
                        // Ignore failed receive and return the message to the queue
                        // cap() = 0 when message was clear()'d, if message was ACTUALLY 0 length
                        // then length is zero but not capacity.
                        // We need to forward zero length messages because they indicate close
                        // of a unix socket.
                        msg.allocate_uninitialized(BUFFER_CAP).unwrap();
                        batch.push_back(msg);
                        continue;
                    }
                    let mlen = msg.len();
                    self.recv_worker_set_state(n, RecvWorkerState::IfaceSend);
                    match self.iface.send(msg) {
                        Ok(()) => {
                            log::trace!("Socket receiver thread sent packet of len {mlen}");
                        },
                        Err(e) => {
                            log::debug!("Error processing packet: {e}");
                        }
                    }
                    if mlen == 0 {
                        // In the event that the socket was disconnected, we receive a 0 length
                        // message every time we poll. This is handled upstream by dropping the
                        // iface, but that only causes the drop of a broadcast which needs to be
                        // polled in the worker thread.
                        //
                        // Without yielding here, this can go into a busyloop because none of the
                        // awaits here are actually waiting at all.
                        self.recv_worker_set_state(n, RecvWorkerState::Yield);
                        tokio::task::yield_now().await;
                    }
                } else {
                    log::error!("Number of messages received does not match - should not happen");
                }
            }
        }
    }
}

trait SocketIfaceInternalT: Send + Sync {
    fn worker_states(&self) -> (Vec<SendWorkerState>,Vec<RecvWorkerState>);
}
impl<T: AsRawFd + Sync + Send> SocketIfaceInternalT for SocketIfaceInternal<T> {
    fn worker_states(&self) -> (Vec<SendWorkerState>,Vec<RecvWorkerState>) {
        let mut rout = Vec::with_capacity(self.recv_worker_states.len());
        let mut sout = Vec::with_capacity(self.send_worker_states.len());
        for r in &self.recv_worker_states {
            let n = r.load(std::sync::atomic::Ordering::Relaxed);
            let x = match RecvWorkerState::try_from(n) {
                Ok(x) => x,
                Err(_) => RecvWorkerState::Invalid,
            };
            rout.push(x);
        }
        for s in &self.send_worker_states {
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

pub struct SocketIface {
    pub iface: Iface,
    // This is never sent to, it is DROPPED in order to cause the tasks to exit
    _done: tokio::sync::broadcast::Sender<()>,
    inner: Arc<dyn SocketIfaceInternalT>,
}
impl Drop for SocketIface {
    fn drop(&mut self) {
        println!("SocketIface drop");
    }
}
impl SocketIface {
    pub fn new<T: AsRawFd + Sync + Send + 'static>(mut fds: Vec<T>, st: SocketType) -> Result<Self> {
        if fds.is_empty() {
            anyhow::bail!("Cannot create a SocketIface with no file descriptors");
        }
        let mut afds = Vec::with_capacity(fds.len());
        for fd in fds.drain(..) {
            afds.push(AsyncFd::new(fd)?);
        }
        let (tgo, tgo_r) =
            tokio::sync::mpsc::channel(TO_GO_OUT_QUEUE);
        let (_done, done_r) =
            tokio::sync::broadcast::channel(1);
        let (mut iface, iface_pvt) = iface::new("SocketIface");

        // Lets assume that in general, we're going to have 2 interfaces with one receiving
        // and the other one sending, both as fast as they can... e.g. TUN / UDP
        // In this scenario, half the core count will be receiving and the other half will be
        // sending.
        let workers = match num_cpus::get() / 2 {
            n if n < fds.len() => fds.len(),
            n => n,
        };

        let out: Arc<SocketIfaceInternal<T>> = Arc::new(SocketIfaceInternal {
            iface: iface_pvt,
            afds,
            st,
            to_go_out_recv: Mutex::new(tgo_r),
            to_go_out_send: tgo,
            done_r,
            send_worker_states: (0..workers).map(|_|AtomicI32::new(0)).collect(),
            recv_worker_states: (0..workers).map(|_|AtomicI32::new(0)).collect(),
        });
        iface.set_receiver(Arc::clone(&out));

        for i in 0..workers {
            tokio::task::spawn(Arc::clone(&out).worker(i, true));
            tokio::task::spawn(Arc::clone(&out).worker(i, false));
        }

        Ok(Self{ iface, _done, inner: out, })
    }

    pub fn worker_states(&self) -> (Vec<SendWorkerState>,Vec<RecvWorkerState>) {
        self.inner.worker_states()
    }
}