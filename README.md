# Hyperboria/docs

## What is Hyperboria?

Hyperboria is test network built of cjdns nodes.

## What is cjdns?

Cjdns is an experimental, cryptographic mesh networking suite.

## What is notable about cjdns? Why should I use it?

Cjdns builds an [end-to-end encrypted](https://en.wikipedia.org/wiki/End-to-end_encryption) [IPv6](https://en.wikipedia.org/wiki/IPv6) [mesh network](https://en.wikipedia.org/wiki/Mesh_networking) that utilizes the [fc00::/8 address space](https://en.wikipedia.org/wiki/Unique_local_address).

### Encryption

Cjdns provides an encrypted tunnel which utilizes a [private/public keypair](https://en.wikipedia.org/wiki/Public-key_cryptography) to encrypt everything that passes through it with the [SALSA20 stream cypher](https://en.wikipedia.org/wiki/Salsa20), which affords the user [perfect forward secrecy](https://en.wikipedia.org/wiki/Forward_secrecy).

### Address Allocation

The ipv6 assigned to the TUN interface is composed of the first 16 bytes of the SHA512 hash of the [SHA512 hash](https://en.wikipedia.org/wiki/SHA-2) of your public key. Keypairs are generated via a [brute force method](https://en.wikipedia.org/wiki/Brute-force_search) until a corresponding address is found with a starting byte FC. The FC00::/8 address space has been allocated as a Unique Local Address space, and so these addresses should not conflict with ICANN assigned IPv6 addresses or any other conventional internet operation.

### Hybrid topology

Cjdns was designed to be used with a [friend of a friend](https://en.wikipedia.org/wiki/Friend_of_a_friend) topology. It builds an [overlay network](https://en.wikipedia.org/wiki/Overlay_network) which traverses NAT ([Network Address Translation](https://en.wikipedia.org/wiki/Network_address_translation)), exposing all ports of every node to every other node within the network. Those who are used to relying on NAT to protect their devices may find this troublesome.

At one point it was expected that each person who peered would do so with only trusted friends. You need not trust relaying nodes with the confidentiality or integrity of your packets, however, if you wish to restrict access to certain services, it is your responsibility to implement effective access control rules.

Links between nodes may be established over deliberate [UDP](https://en.wikipedia.org/wiki/User_Datagram_Protocol) links across the internet, or over deliberate or automatic connections across Ethernet, Wireless access points, Ad-hoc connections, or various system-specific transmission and addressing protocols.

### Cryptographic verification and routing

Establishing a link between two nodes includes a cryptographic authorization process, after which other nodes within the network can establish a connection with the new node.

Due to the relationship between each node's public key and its ipv6, and the fact that this relationship is verified upon connecting to a node, a user can be sure that if it connects to an IPv6 address and receives a response then the node that responded possesses the requisite private key to decrypt the response.

If you are able to connect at all over cjdns, then you can be quite sure that the node you connected to possesses the private key that corresponds to the IPv6.

## How can I get involved?

If you are totally new to cjdns, you'll want to start by installing it somewhere and familiarizing yourself with how it works. Take notes of anything that you find difficult, unintuitive, or poorly documented, and share your knowledge so the next person who installs cjdns after reading the documentation has an easier time.

Also, read our [contributing](contributing.md) document for notes on our policies for accepting contributions.
