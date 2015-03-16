#Security Specification

This document is here to help define security flaws in a way that can be agreed upon.

##Definitions

* Reveal: To reveal a piece of content is to send over the wire that content or something from which
that content can be mathematically derived (not withstanding theoretical attacks
eg: Quantum Computers unless explicitly said otherwise)

* Matching Keypair: In relation to an IPv6 address, a public and private key for which the first
16 bytes of the double-SHA-512 (the SHA-512 of the SHA-512) of the public key matches the IPv6
address.

* Permanent Private Key: The key which is in the configuration file which defines the node's
identity.

##Limitation of Scope

This document applies to the current version (latest master branch) of cjdns compiled and running
on a Linux system using a version of GCC with no known security issues and running on an
x86 or amd64 processor. Older versions and git branches may be non-compliant with this
specification.

##Integrity and Confidentiality

1. It must not be possible to reveal the permanent private key or any temporary session private keys
to anyone.

2. It must not be possible to reveal any temporary symmetrical session keys to anyone other than the
identity (permanent public key) with which the session has been established.

3. It must not be possible to reveal the content of any packet with an fc00::/8 destination address
which enters cjdns through the TUN device, except revealing it to the holder(s) of the Matching
Keypair for that address.

4. It must not be possible to receive any packet with an fc00::/8 source address unless the sender
is in possession of the Matching Keypair for that address and the content (not including fields in
the IPv6 header other than source and destination addresses) has not been altered en-route.

5. It must not be possible to receive a packet which is part of an ESTABLISHED session of that
packet was already received before.

6. If the permanent private key is compromised, any traffic which was sent during a previous
ESTABLISHED session which has since ended will not be compromised as a result.

NOTE: Traffic may also be sent when a session is in HANDSHAKE state and for traffic sent while the
session is in HANDSHAKE state, 5 and 6 may be violated.

7. In the event of a cryptographic break of Curve25519 ECDH (for example using the Shor attack with
a Quantum Computer) any cryptographic session which was initiated using a password will remain
as strong as a session which had been initiated using that password as a symmetrical key.


