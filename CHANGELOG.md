# Cjdns Changelog

## Version 22.1 - Seedy
January 8, 2025

This is a point version so there are no changes to the inter-node protocol but introduces a number of new features.

### Major Changes
1. DNS Seeding allows auto-peering, so manually adding peers is now optional, new RPCs:
  - `PeeringSeeder_publicStatus`: Shows status
  - `PeeringSeeder_publicPeer`: Enables becoming a public node
  - `PeeringSeeder_listDnsSeeds`: List the active DNS seed nodes
  - `PeeringSeeder_rmDnsSeed`: Remove a DNS seed node
  - `PeeringSeeder_addDnsSeed`: Add a DNS seed node
2. cjdnstool is compiled with cjdroute making it easier to do node administration.

### Minor Changes
1. `Security_seccomp` removed, SECCOMP BPF filter because is too strict for the 3rd party Rust libraries.
2. Libuv is gone from the project, all interfaces with the OS are through Rust code.
3. UDP and TUN interfaces now use tokio worker pools, new RPCs:
  - `UDPInterface_workerStates`: Check the status of UDP Interface workers.
  - `Core_tunWorkers`: Check the status of TUN Interface workers.

## Version 22 - Noisemaker
February 3, 2023

It's been since September 18, 2020 when cjdns v21 was tagged out, and brought only small experimental inclusion of
Rust code to cjdns. v22 makes Rust a main language in which cjdns is written, and it is foreseen that in v23, almost
all interactions with the OS will be done in Rust.

### Major Changes
1. New protocol version v22, compatibility is maintained with v21 and v20 (no change in compatibility)
  * This new protocol version brings a new CryptoAuth protocol based on the NOISE protocol used by WireGuard® VPN.
  The NOISE protocol is used in all direct peering connections where both sides are v22.
  * The legacy CryptoAuth protocol remains but has been entirely re-written in Rust,
  the C version remains only for testing purposes.
2. RPC `InterfaceController_peerStats()` Now contains Integer noiseProto: 1 if using the NOISE protocol for communication
3. RPC `InterfaceController_disconnectPeer()` Now disconnects ALL sessions to given peer address instead of only the first
4. RPC `InterfaceController_timestampPackets()` has been removed, as it only affected debug logging and was rarely used
5. Libuv code is now compiled using the cjdns build system, it is nolonger compiled using gyp so **python is nolonger needed to build cjdns**
6. Removed `cjdroute --bench` which was not an accurate measure of expected performance

### Minor Changes
1. Made fields of Message structure so they will be accessed through accessors - first step to move Message into Rust
2. Make cjdns Error return value use a Rust anyhow error
3. In preparation for removal of Libuv, we have replaced the following Libuv functions with Rust implementations:
  * IP Address manipulation functions: `uv_inet_pton` / `uv_inet_pton`
  * Current time: `uv_now` / `uv_hrtime`
  * Child process: `uv_spawn` / `uv_exepath` / `uv_process_kill`
4. We nolonger put `makekeys`, `mkpasswd`, `privatetopublic`, `publictoip6`, `randombytes`, and `sybilsim` in the main folder
  * In the future: `makekeys`, `privatetopublic`, `publictoip6`, and `randombytes` will be built as a separate optional component
  * `mkpasswd` will be dropped because it is too trivial to be worth maintaining
  * `sybilsim` will be dropped because it is nolonger maintained
5. Removed from SwitchCore.c the possibility of an interface to be "down" which never exists in reality
6. New logger for Rust code which uses the cjdns logger as a backend
7. CryptoAuth is now an (asynchronous) Iface rather than a function you call to encrypt and decrypt

# Older versions
For earlier versions, see ./util/version/Version.h