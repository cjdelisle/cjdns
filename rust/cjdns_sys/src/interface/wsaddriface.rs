use futures_util::stream::SplitStream;
use futures_util::{SinkExt, StreamExt};
use num_enum::{IntoPrimitive, TryFromPrimitive};
use rustls::{DigitallySignedStruct, SignatureScheme};
use tokio::io::{AsyncRead, AsyncWrite};
use tokio::net::{TcpListener, TcpStream};
use tokio::select;
use tokio::sync::mpsc::{self, Sender};
use tokio::sync::oneshot;
use tokio_tungstenite::tungstenite::handshake::server::Request;
use tokio_tungstenite::tungstenite::protocol::WebSocketConfig;
use tokio_tungstenite::tungstenite::Message as WsMessage;
use tokio_tungstenite::{
	accept_hdr_async_with_config,
	connect_async_with_config,
	connect_async_tls_with_config,
	WebSocketStream,
	Connector,
};
use rustls::client::danger::{HandshakeSignatureValid, ServerCertVerified, ServerCertVerifier};
use rustls::pki_types::{CertificateDer, ServerName, UnixTime};
use rustls::Error as TLSError;

use crate::external::interface::iface::{self, IfRecv, Iface, IfacePvt};
use crate::interface::socketiface::RecvWorkerState;
use crate::interface::wire::message::Message;
use crate::util::{now_ms, sockaddr};
use crate::util::sockaddr::Sockaddr;
use eyre::{Context, Result};
use std::collections::HashMap;
use std::convert::TryFrom;
use std::net::SocketAddr;
use std::sync::atomic::Ordering::Relaxed;
use std::sync::atomic::{AtomicI32, AtomicU32};
use std::sync::Arc;
use std::time::Duration;

const TO_GO_OUT_QUEUE: usize = 64;

/// Headroom left at the front of incoming Messages so that the Sockaddr and
/// any further downstream headers can be pushed without reallocation.
const PADDING_AMOUNT: usize = 512;

/// Maximum accepted WebSocket message size, matching the UDP BUFFER_CAP.
/// Frames larger than this are rejected by tungstenite during read.
const MAX_FRAME_SIZE: usize = 3496;

#[derive(Debug, IntoPrimitive, TryFromPrimitive)]
#[repr(i32)]
pub enum SendWorkerState {
	Invalid = -1,
	Initializing = 0,
	WaitLock = 1,
	RecvBatch = 2,
	Dispatch = 3,
}

fn ws_config() -> WebSocketConfig {
	let mut cfg = WebSocketConfig::default();
	cfg.max_message_size = Some(MAX_FRAME_SIZE);
	cfg.max_frame_size = Some(MAX_FRAME_SIZE);
	cfg
}

/// One live WebSocket connection (incoming or outgoing).
struct WsConn {
	last_recv_sec: AtomicU32,

	send: Sender<Message>,

	peer: String,

	recv_worker_state: AtomicI32,
	send_worker_state: AtomicI32,

	done: parking_lot::Mutex<Option<tokio::sync::oneshot::Sender<()>>>,
}
impl WsConn {
	fn new(send: Sender<Message>, done: oneshot::Sender<()>, peer: String) -> Arc<Self> {
		Arc::new(Self {
			last_recv_sec: AtomicU32::new((now_ms() / 1000) as u32),
			send,
			recv_worker_state: AtomicI32::new(RecvWorkerState::Initializing as _),
			send_worker_state: AtomicI32::new(SendWorkerState::Initializing as _),
			done: parking_lot::Mutex::new(Some(done)),
			peer,
		})
	}
	fn notify_received(&self) {
		self.last_recv_sec.store((now_ms() / 1000) as u32, Relaxed)
	}
	fn last_recv_sec(&self) -> u32 {
		self.last_recv_sec.load(Relaxed)
	}
	fn recv_worker_set_state(self: &Arc<Self>, state: RecvWorkerState) {
		self.recv_worker_state.store(state as i32, Relaxed);
	}
	fn send_worker_set_state(self: &Arc<Self>, state: SendWorkerState) {
		self.send_worker_state.store(state as i32, Relaxed);
	}
}

struct WSAddrIfaceInternal {
	iface: IfacePvt,
	conns: parking_lot::RwLock<HashMap<Sockaddr, Arc<WsConn>>>,
	next_id: AtomicU32,
	conn_timeout_secs: u32,
	peer_id: String,
}

impl IfRecv for Arc<WSAddrIfaceInternal> {
	fn recv(&self, mut m: Message) -> Result<()> {
		let sa = Sockaddr::try_from(m.bytes()).context("Getting address from message")?;
		m.discard_bytes(sa.byte_len())?;
		if let Some(wsconn) = self.conns.read().get(&sa) {
			if wsconn.last_recv_sec() < self.oldest_allowed() {
				// Drop out and attempt reconnect
			} else if let Err(e) = wsconn.send.try_send(m) {
				log::debug!("DROP unable to send to {} WS worker: {e}",
					if let Some(id) = sa.as_handle() {
						id.to_string()
					} else if let Some(url) = sa.as_url() {
						url
					} else {
						"<UNKNOWN>".to_string()
					}
				);
				return Ok(());
			} else {
				return Ok(());
			}
		}
		if let Some(id) = sa.as_handle() {
			log::debug!("DROP {id} incoming WS worker timed out");
		} else if sa.type_of() == sockaddr::TYPE_URL {
			tokio::spawn(Arc::clone(self).connect_send(sa, m));
		} else {
			log::warn!("DROP unable to WS worker, unexpected sockaddr type");
		}
		Ok(())
	}
}

#[derive(Debug)]
struct NoVerifier;

// Credit to reqwest for this:
impl ServerCertVerifier for NoVerifier {
    fn verify_server_cert(
        &self,
        _end_entity: &rustls_pki_types::CertificateDer,
        _intermediates: &[rustls_pki_types::CertificateDer],
        _server_name: &ServerName,
        _ocsp_response: &[u8],
        _now: UnixTime,
    ) -> Result<ServerCertVerified, TLSError> {
        Ok(ServerCertVerified::assertion())
    }

    fn verify_tls12_signature(
        &self,
        _message: &[u8],
        _cert: &rustls_pki_types::CertificateDer,
        _dss: &DigitallySignedStruct,
    ) -> Result<HandshakeSignatureValid, TLSError> {
        Ok(HandshakeSignatureValid::assertion())
    }

    fn verify_tls13_signature(
        &self,
        _message: &[u8],
        _cert: &rustls_pki_types::CertificateDer,
        _dss: &DigitallySignedStruct,
    ) -> Result<HandshakeSignatureValid, TLSError> {
        Ok(HandshakeSignatureValid::assertion())
    }

    fn supported_verify_schemes(&self) -> Vec<SignatureScheme> {
        vec![
            SignatureScheme::RSA_PKCS1_SHA1,
            SignatureScheme::ECDSA_SHA1_Legacy,
            SignatureScheme::RSA_PKCS1_SHA256,
            SignatureScheme::ECDSA_NISTP256_SHA256,
            SignatureScheme::RSA_PKCS1_SHA384,
            SignatureScheme::ECDSA_NISTP384_SHA384,
            SignatureScheme::RSA_PKCS1_SHA512,
            SignatureScheme::ECDSA_NISTP521_SHA512,
            SignatureScheme::RSA_PSS_SHA256,
            SignatureScheme::RSA_PSS_SHA384,
            SignatureScheme::RSA_PSS_SHA512,
            SignatureScheme::ED25519,
            SignatureScheme::ED448,
        ]
    }
}


impl WSAddrIfaceInternal {

	fn oldest_allowed(&self) -> u32 {
		(now_ms() / 1000) as u32 - self.conn_timeout_secs
	}

	async fn connect_send(self: Arc<Self>, sa: Sockaddr, m: Message) {
		let Some(url) = sa.as_url() else {
			log::error!("Got connect_send with non-url SA");
			return;
		};
		match self.connect_send1(sa, m, &url).await {
		    Ok(()) => (),
		    Err(e) => {
		        log::debug!("DROP Unable to connect to WS: {url}: {e}");
			return;
		    }
		};
	}

	async fn connect_send1(self: Arc<Self>, sa: Sockaddr, m: Message, url: &String) -> Result<()> {
		let mut full_url = url::Url::parse(&url)?;
		full_url.query_pairs_mut().append_pair("cjdns-peer-id", &self.peer_id);

		let (ws, _resp) = if url.starts_with("wss://") {
			let config = rustls::ClientConfig::builder()
				.dangerous()
				.with_custom_certificate_verifier(Arc::new(NoVerifier))
				.with_no_client_auth();
			connect_async_tls_with_config(
				full_url.as_str(),
				Some(ws_config()),
				true,
				Some(Connector::Rustls(Arc::new(config))),
			).await?
		} else {
			connect_async_with_config(
				full_url.as_str(),
				Some(ws_config()),
				true
			).await?
		};

		self.run_conn(ws, Some(sa), url.clone(), Some(m)).await;
		Ok(())
	}

	async fn poll_stream<S>(
		self: &Arc<Self>,
		wsconn: &Arc<WsConn>,
		mut stream: SplitStream<WebSocketStream<S>>,
		sa: &Sockaddr,
	) where
		S: AsyncRead + AsyncWrite + Unpin + Send + 'static,
	{
		loop {
			wsconn.recv_worker_set_state(RecvWorkerState::RecvBatch);
			let Some(res) = stream.next().await else {
				break;
			};
			wsconn.recv_worker_set_state(RecvWorkerState::IfaceSend);
			let data = match res {
				Ok(WsMessage::Binary(data)) => data,
				Ok(WsMessage::Ping(p)) => {
					log::info!("WS got ping");
					continue;
				}
				Ok(WsMessage::Pong(_)) => {
					log::info!("WS got pong");
					continue;
				}
				Ok(WsMessage::Close(_)) => {
					log::debug!("WS peer sent close");
					break;
				}
				Ok(_) => {
					// Text and raw frames are not part of this protocol; ignore.
					continue;
				}
				Err(e) => {
					log::info!("WS read error: {e}");
					break;
				}
			};
			wsconn.notify_received();
			log::trace!("Ok WS frame with {} bytes", data.len());
			let mut msg = Message::new(PADDING_AMOUNT + data.len());
			if let Err(e) = msg.push_bytes(&data) {
				log::warn!("Error building message from WS frame: {e}");
				continue;
			}
			if let Err(e) = msg.push_bytes(sa.bytes()) {
				log::warn!("Error pushing Sockaddr onto message: {e}");
				continue;
			}
			match self.iface.send(msg) {
				Ok(()) => {
					log::trace!("WS receiver sent packet successfully");
				}
				Err(e) => {
					log::debug!("Error processing packet: {e}");
				}
			}
		}
	}

	fn replace_conn(self: &Arc<Self>, sa: &Option<Sockaddr>, wsconn: Arc<WsConn>) -> Sockaddr {
		let mut remove = Vec::new();
		let oa = self.oldest_allowed();
		let mut out = None;
		let mut l = self.conns.write();
		l.retain(|k, conn| {
			if conn.last_recv_sec() < oa {
			} else if let Some(sa) = sa {
				if k == sa {
					out = Some(k.clone());
				} else {
					return true;
				}
			} else if conn.peer == wsconn.peer {
				out = Some(k.clone());
			} else {
				return true;
			}
			remove.push(Arc::clone(conn));
			false
		});
		let out = if let Some(out) = out {
			out
		} else if let Some(sa) = sa {
			sa.clone()
		} else {
			let id = self.next_id.fetch_add(1, Relaxed);
			println!("\n\nCREATING HANDLE {id}\n\n");
			Sockaddr::from(id)
		};
		l.insert(out.clone(), wsconn);
		drop(l);
		for a in remove {
			// Drop sender, receiver exits
			a.done.lock().take();
		}
		out
	}

	/// Runs one established connection to completion: splits the stream,
	/// spawns a writer task fed by the connection's queue, and reads frames
	/// until close/error. Generic so it serves both plain (incoming) and
	/// possibly-TLS (outgoing) streams.
	async fn run_conn<S>(
		self: Arc<Self>,
		ws: WebSocketStream<S>,
		sa: Option<Sockaddr>,
		peer: String,
		msg: Option<Message>,
	) where
		S: AsyncRead + AsyncWrite + Unpin + Send + 'static,
	{
		let (send, mut recv) = mpsc::channel(TO_GO_OUT_QUEUE);
		if let Some(msg) = msg {
			// Enqueue initial message
			let _ = send.send(msg).await;
		}

		let (done, is_done) = oneshot::channel();

		let wsconn = WsConn::new(send, done, peer);

		let sa = self.replace_conn(&sa, Arc::clone(&wsconn));

		let (mut sink, stream) = ws.split();

		let wsconn_sender = Arc::clone(&wsconn);
		tokio::spawn(async move {
			wsconn_sender.send_worker_set_state(SendWorkerState::RecvBatch);
			while let Some(msg) = recv.recv().await {
				wsconn_sender.send_worker_set_state(SendWorkerState::Dispatch);
				let frame = WsMessage::Binary(msg.bytes().to_vec().into());
				if let Err(e) = sink.send(frame).await {
					log::info!("WS {} send error: {e}", wsconn_sender.peer);
					break;
				}
				wsconn_sender.send_worker_set_state(SendWorkerState::RecvBatch);
			}
			wsconn_sender.send_worker_set_state(SendWorkerState::Dispatch);
			let _ = sink.close().await;
			wsconn_sender.send_worker_set_state(SendWorkerState::Invalid);
		});

		select! {
			_ = self.poll_stream(&wsconn, stream, &sa) => {
				log::debug!("Websocket closed (remote)");
			},
			_ = is_done => {
				log::debug!("Websocket closed (local)");
			}
		};

		// If the conn is still in the table, remove it
		let mut l = self.conns.write();
		if let Some(conn) = l.remove(&sa) {
			if !Arc::ptr_eq(&conn, &wsconn) {
				l.insert(sa, conn);
			}
		}
		drop(l);
	}

	async fn accept(self: Arc<Self>, stream: TcpStream, peer: SocketAddr) {
		let mut peer_id = None;
		let cb = |req: &Request, res|{
			peer_id = req
				.uri()
				.query()
				.and_then(|q| {
					url::form_urlencoded::parse(q.as_bytes())
						.find_map(|(k, v)| {
							if k == "cjdns-peer-id" {
								Some(v.into_owned())
							} else {
								None
							}
						})
				});
			Ok(res)
		};
		stream.set_nodelay(true);
		let res = accept_hdr_async_with_config(stream, cb, Some(ws_config())).await;
		let ws = match res {
			Ok(ws) => ws,
			Err(e) => {
				log::info!("WS handshake failed from {peer}: {e}");
				return;
			}
		};
		let Some(peer_id) = peer_id else {
			log::debug!("WS request from {peer} with no peer_id");
			return;
		};
		log::debug!("Incoming WS connection from {peer} with id {peer_id}");
		self.run_conn(ws, None, peer_id, None).await
	}

	/// Accept loop for incoming connections. Each accepted socket gets a
	/// fresh u32 id which becomes its Sockaddr via `From<u32>`.
	async fn listen_worker(self: Arc<Self>, listener: TcpListener) {
		loop {
		    match listener.accept().await {
		        Ok((stream, peer)) => {
				tokio::spawn(Arc::clone(&self).accept(stream, peer));
			}
		        Err(e) => {
		            log::warn!("WS accept error: {e}");
		            tokio::time::sleep(Duration::from_millis(100)).await;
		        }
		    }
		}
	}
}

pub struct WsAddrIface {
	internal: Arc<WSAddrIfaceInternal>,
	pub local_addr: SocketAddr,
}

impl WsAddrIface {
	pub fn new(bind_addr: &SocketAddr, conn_timeout_secs: u32, peer_id: String) -> Result<(Self, Iface)> {
		let listener = std::net::TcpListener::bind(bind_addr)
		    .with_context(|| format!("Binding WS listener to {bind_addr}"))?;
		listener.set_nonblocking(true)?;
		let listener = TcpListener::from_std(listener)?;
		let local_addr = listener.local_addr()?;

		let (mut iface, iface_pvt) = iface::new("WSAddrIface");

		let internal = Arc::new(WSAddrIfaceInternal {
		    iface: iface_pvt,
		    conns: Default::default(),
		    next_id: AtomicU32::new(0),
		    conn_timeout_secs,
		    peer_id,
		});
		iface.set_receiver(Arc::clone(&internal));

		tokio::task::spawn(Arc::clone(&internal).listen_worker(listener));

		Ok((
		    WsAddrIface {
		        internal,
		        local_addr,
		    },
		    iface,
		))
	}
}
