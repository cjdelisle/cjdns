*This is a work in progress*

# CJD's Networking Suite -- Everyone Still Thinks It's A Nameserver

I started it a year ago when [DNS]-[P2P] was going on, it was going to be a name
server. After writing the base libraries for the project, I realized that
nobody will install alternative DNS because anything that is on the Internet
needs a `.com` for the rest of the world to read it. A website is not going to
be inaccessible to almost everyone, if it were they might as well stop paying
hosting bill. If you have a `.com`, why should you bother with a cjdns address,
you're already accessible. It's a chicken and egg problem. [Namecoin] solved
part of the problem but the only original invention in Namecoin was special
transactions for [Bitcoin]. Namecoin did not bring about a secure and fast
method of looking up address information and the developers don't seem to have
much interest in developing one. So I switched from [DNS] to fixing the low
level protocols because that provides something for everyone and the chicken-
and-egg problem is not so great.

The Internet is a wonderful system but deep down in it's aging protocols there
are some flaws. We all know that we would like cryptographic integrity and
confidentiality guarantees with each of our packets, something that the
government has been working on through [IPSEC] and [DNSSEC], but there are a few
other design problems with the Internet which have manifested themselves as
social problems.

## Censorship via Denial of Service and Frivolous Litigation

Content can be censored by sending a flood of unwanted packets to a host from
a large number of infected "zombie" machines. This, known as [DDoS], is a
problem which worsens every year as the upload speed of all infected nodes on
the Internet grows in proportion to the download speed of any given link.
Being infected with a virus and participating in DDoS, though not a picnic, is
not an emergency for the owner of the infected machine. Nor is it an emergency
for their ISP. DDoS is always "their problem"... Until it strikes in your
network. Sadly, a common response from a datacenter is to stop carrying the
controversial content, making DDoS an effective censorship tool and
encouraging the practice.

Another form of censorship which is even more insidious is frivolous and
malicious court action. While every litigant would swear up and down that he
has been injured and has only the most noble goals at heart, it is not hard to
recognize the power of the court system and it stands to reason that those
with malicious intent would do so as well. Whereas DDoS is a low risk activity
because of the level of anonymity afforded to the savvy and unscrupulous,
malicious litigation is a low risk activity because "I believed I was doing
the right thing" is a near unchallengeable defense. Furthermore, the cost of
bringing litigation as compared to the cost of confronting the embarrassing
truth is very low, while the cost of defending against malicious litigation as
compared to the self gratification from telling the truth is extremely high.
Finally, people are unable to think about litigation risk in the same way as
we think about other risks because there is an assumption that "if you're not
doing anything wrong you won't have any trouble" which history shows us over
and over, couldn't be further from the truth.

## Space Deaggregation

A more technical issue with the Internet, and one of which many people are
unaware, is address space deaggregation. Every machine connected to the
Internet needs an address, you may have heard the IPv6 lobby talking about
"address space exhaustion" which, while a concern, is not nearly as dire as
they make it out to be. Only about 20% of the allocated address space is
announced to the routing table (giving those addresses access to the Internet)
and there is a significant amount of space which the numbers authorities have
decided not to allocate at all. However, at every stop along it's path through
the Internet, a packet has it's address field examined by a router so it can
decide which wire that packet should be sent down. Routers have an easier time
if addresses are in big blocks so that a router can look quickly at the first
numbers in the address and know, for example, that it is destined for China.
People naturally want as many addresses as they can possibly get and they want
them in the smallest blocks possible, this is so they can then control (or buy
and sell) these small blocks independently. The smaller the blocks of
addresses which are announced, the larger the routing tables become and the
more work the Internet's core routers must do in order to send a packet in the
right direction. There have been attempts to aggregate addresses back in to
groups but nonetheless, the number of small announcements in the global
routing table has grown every year.

Each of these problems is a tragedy of the commons problem. The users of virus
infected computers are incentivized to save money rather than purchasing a
product or service to rid their computer of the infection. While there are
solutions such as egress filtering which decrease the problem, ISPs are
incentivized to implement as little security as possible because they are not
directly affected. Frivolous and malicious litigation benefits the plaintiff
who is able to hide the truth and the lawyers, judges, and other court
officers whose services are demanded. The defendant who all too often
publishes information for no other reason than satisfaction of telling the
truth, is the only party harmed by this type of attack, and he is the least
able to prevent it. Address space deaggregation benefits the edge ISPs who
gain more flexibility in how their network is organized at the cost of the
core ISPs whose only defense is the "we will not route that" nuclear option
which would no doubt bring about a revolt from the edge ISPs. Each of these
problems hurts everyone, DDoS forces ISPs to over prevision their lines,
frivolous legal action increases the cost of running a community website or
ISP since each accusation must be reviewed and it's validity assessed, and
address deaggregation means everyone must pay more to have their packets
routed through increasingly high power routers.

Steps have been taken to address each of these problems but these steps have
been largely patches against the existing flawed system. The most popular
anti-DDoS measure is bandwidth overprovisioning, it is not unusual for large
companies to spend 75% more on extra bandwidth to handle unexpected traffic
including DDoS.[1]  Mixnets such as TOR, I2P, Freenet, and Phantom attempt to
solve the problem of frivolous litigation (and other invalid suppression of
speech) and as a side effect, provide DDoS resistance.

## Mixnets

### Tor

[TOR][Tor] is a relatively popular anonymity system which functions by randomly
routing encrypted traffic through nodes, the nodes don't know what they are
routing and no one node knows both source of the traffic and it's destination.
TOR's abuse policy is "it's all just zeros and ones", this has made it popular
for sharing child abuse content and it has been known to host at least one
drive-by-download malware attack. Furthermore, TOR nodes have an option of
proxying back to the external Internet, those who enable this are called "exit
nodes". These, like other proxies, are popular for evading ip address bans
such that most large IRC networks choose to block all TOR nodes from access
unless the user authenticates. Google is also known to prevent TOR exit nodes
from running searches unless the users periodically complete captchas. TOR's
nonexistant abuse policy not only enables criminals but it drives away people
who believe in freedom of speech but not to the extent that it is coordinating
criminal activity. Since the users of TOR get their bandwidth from that
donated by the operators of transit nodes and as an inherently anonymous
protocol, there are no fairness metrics. Users are incentivized to use as much
bandwidth as possible which is generally a matter of opening vast numbers of
concurrent connections. One user reported downloading 10 megabits per second
of [BitTorrent] traffic through TOR. The fact that TOR nodes are associated with
abuse taking place on the public Internet and TOR's centralized directory
server system mean that getting a list of TOR nodes is relatively easy to do
so authoritarian governments, the entities which TOR advocates claim it
thwarts, need only filter access to the nodes on the list (or round up those
who connect to them). TOR is written in C and requires little for resources
except for TCP connections which it uses to communicate with other nodes.
TOR's interface with the user is a reasonably easy to use socks proxy which
allows for TCP functions such as web browsing but is not a full replacement
for the Internet.

### I2P

[I2P] is a less popular protocol similar to TOR, I2P has no means to access the
outside Internet anonymously and therefor has not created as much publicity.
I2P's abuse policy is, like TOR's, nonexistant. It has however remained mostly
clean of criminality, probably due to it's lower popularity. Like TOR, I2P
routes packets randomly around the Internet which not only requires the
Internet layer below it but also leads to excessive latency and inefficient
routing. Unlike TOR, I2P contains fairness metrics which prevent blatant
bandwidth abuse. I2P is written in Java and uses enough resources that it is
unsuitable for use in any small device or low end server. I2P's user interface
is like TOR's, a simple socks proxy which allows for web browsing and other
basic web functions. I2P contains a library which allows applications to
communicate over I2P if modifications are made to the application's code.


### Freenet

[Freenet] is a rather interesting protocol, like TOR, it runs over the existing
Internet and anyone can join. Like I2P, it does not proxy out to the outside
Internet. Unlike TOR and I2P it is a distributed store so people who insert
content into the network can then turn their computers off and the content
remains available. As a data store it has been known to host child abuse
material and in addition, would be node operators are faced with the fact that
they are storing random Freenet content on their harddrive which while likely
not a crime, is morally difficult to reconcile. Also since data is stored on
nodes, Freenet is vulnerable attacks by inserting large amounts of garbage
data or spam which would displace legitimate data. As a content addressable
Internet it is very interesting, it contains static content (stored by hash)
and mutable content (stored by key). It also has a "darknet" mode which allows
nodes to peer privately without allowing other nodes to connect to them so it
can contain highly secretive nodes. Freenet's content distribution is far more
DDoS resistant than I2P or TOR and it has the added benefit that a
denationalizing attack involving DDoSing each node while polling a hidden site
to see when it becomes unavailable is not effective. Freenet employs small
world routing and routes randomly when a node cannot be found in order to
maximize anonymity. Like TOR and I2P, Freenet uses a socks proxy to interface
with the user. Unlike TOR and I2P, Freenet is highly restrictive of the type
of website which it can host. New kinds of content placed in the Freenet
network requires a new way of using the static/mutable store and a new
permissions system. Running a webapp is simply impossible.


### Phantom 

[Phantom] is also an interesting protocol. Like TOR it is written in C, like I2P
it does not proxy back to the Internet and unlike any of the other protocols
described, it interfaces with the user as a TUN device (a virtual network
card). This allows for all types of Internet traffic to be sent through
Phantom, not just simple web browsing. Phantom shows a lot of promise as a
faster anonymity protocol but it is still very new so very little is known
about how it behaves in the real world.

### Mixnets In General

All of these protocols are meant to run over the existing Internet, all of
them connect peers automatically using a discovery system and all of them use
suboptimal routing paths to bolster anonymity at the cost of performance. To
address the growing routing tables, Cisco has proposed a new protocol called
Locator/Identifier Separation Protocol or LISP. The idea of LISP is to
separate the addresses which people use from the addresses which routers use,
like a lower level version of DNS. LISP allows the edge ISPs to see the
Internet as they want it, disaggregated into small pieces for political
reasons, and the routers to see the Internet as they want it, centralized
hierarchical pyramid of addresses emanating from some arbitrary center point.
This design works well in existing routers since they are designed to get
packets with a universally unique address and look that address up in a table.
This is advertised as a design feature but LISP is limited in it's vision, if
one must look up the "real location" of a server before forwarding a packet,
why not simply look up the fastest path?

## A New vision

Imagine an Internet where every packet is cryptographically
protected from source to destination against espionage and forgery, getting an
IP address is as simple as generating a cryptographic key, core routers move
data without a single memory look up, and DDoS is a term read about in history
books. The cost of frivolous legal action is higher and the cost of operating
a service is lower because identifying users requires cooperation from network
operators but these same network operators work together to enforce a
community defined acceptable use policy. Finally, becoming an ISP is no longer
confined to the mighty telecoms, anyone can do it by running some wires or
turning on a wireless device.

This is the vision of cjdns. 

cjdns is designed to be a "principle of least authority" protocol, on the
Internet, information is authority so therefore it is a "need to know"
protocol. Those who don't need to know a piece of information don't know. Data
is encrypted twice using the fast `curve25519`, `salsa20` and `poly1305` algorithms.
Data is encrypted from source to destination making it difficult to forge a
random packet with valid checksums and unfeasible to forge a packet with
arbitrary data. For users who need greater assurance, `poly1305` and replay
protection can be enabled for an extra 16 bytes of overhead per packet. Each
router along the path encrypts the packet once again to protect the IP header
from the array of switches between them and the next router. Finally, the
switches, depending on their connection medium, may apply yet a third layer of
cryptography in order to protect switch headers and prevent forging or
replaying of high priority packets.

Each cjdns "node" is a router and a switch. Switches are the cjdns engine's
analog of the OSI level 2. They are very dumb and only know how to read a
label, parse the lowest bits in that label and forward the packet down the
interface given by those bits. The lack of any memory look ups makes switches
very fast as well. Switches use an internal numeric compression scheme to
compress the interface index into a few bits of the 64-bit label. How they
compress the number is an implementation detail as long as they can read a
label number and know how many of the low bits belong to them. After
determining the correct destination interface, the switch will bit shift the
label and add bits to the now empty high side of the label such that if the
label were reversed, the switch would send the packet back to the interface
where it came from. In the event of an error, the switch does a bitwise
reversal of the label and sends the packet to the source interface. By doing a
full bitwise reversal, the switches need not care how other switches encode
the numbers or be able to reverse the order since they can reverse the order
of the entire label.

                        1               2               3
        0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     0 |                                                               |
       -                         Switch Label                          -
     4 |                                                               |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     8 |  Type |  Frag |                  Priority                     |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

Switch headers are designed to be small and efficient. The fields include the
`label` of which some unknown number of bits, henceforth known as a
discriminator, belong to each switch along the forwarding path, the `Type` field
indicating the type of packet. Reserved packet types are `0` for opaque data,
and `1` for switch control messages (eg errors). The `Frag` field which tells the
recipient what number fragment is being carried under this switch header if
fragmentation is supported. The `Frag` field are only of concern to the sender
and recipient. The `Priority` field contains a number which represents how
important the delivery of a packet is. When a link becomes saturated, the
switch sending packets over that link SHOULD drop packets of least priority
and MAY decrease the priority of all packets passing through it. When packets
are dropped, switches SHOULD emit an error packet with the inverse label to be
sent to the sender. Switches SHOULD make adjustments based on error packets
which are sent in response to packets which they forwarded, forwarding error
packets is OPTIONAL, in flood situations it may not be wise.

If Alice wants to send a packet to Fred via Bob, Charlie, Dave and Elinor, she
will send a message to Bob's switch which has a label that causes the packet
to be routed to Charlie then on to Dave.

NOTE: Spaces between bits are for illustration only, switches do not know how
many bits of a label are used by any other switch than themselves.

Alice's original label:

    0000000000000000000000000000000000000 0101011 011010 100101101 10111
    ^^^^^^^^^-- unused space --^^^^^^^^^^                          ^^^^^-- Bob's discriminator for routing to Charlie.

Bob shifts off his discriminator and applies to the top of the label the bit-
reversed discriminator for the Bob->Alice interface.

    11001 0000000000000000000000000000000000000 0101011 011010 100101101
    ^^^^^-- Bob's discriminator for Alice, bit-reversed.       ^^^^^^^^^-- Charlie's discriminator for Dave.

Charlie removes his discriminator and applies the reversed discriminator for sending to Bob then forwards to Dave.

    100101101 11001 00000000000000000000000000000000000000 101011 011010
    ^^^^^^^^^ ^^^^^-- Bob's discriminator for Alice (reversed).   ^^^^^^-- Dave's discriminator for Elinor.
            ^-- Charlie discriminator for Bob (reversed).

Supposing Dave cannot forward the packet and needs to send an error, he does
not know where Charlie's discriminator ends and Bob's begins so he can't re
order them but because they are bit reversed, he can reverse the order by bit
reversing the entire label.

    010110 110101 00000000000000000000000000000000000000 10011 101101001
                                                         ^^^^^ ^^^^^^^^^-- Charlie'sdiscriminator for Bob.
                                                         ^-- Bob's discriminator for Alice.

Dave can then send the packet back to Charlie who need not know what it is in
order to forward it correctly on to Bob and then to Alice.

When Alice's router asks Bob's for the route to Charlie's router, Bob MUST
give a route (`routeBC`) which, when shifted left by one less than the position
of the most significant set bit in the route from Alice to Bob (`routeAB`) and
XORd against `routeAB`, will yield a route which leads from Alice to Charlie via
Bob.

Given:

    routeAB =      0000000000000000000000000000000000000000000001011101110101011001
    routeBC =      0000000000000000000000000000000000000000000000000000110101010101
    
    SHIFT  routeBC 0000000000000000000000000000000000110101010101000000000000000000
    XOR    routeAB 0000000000000000000000000000000000000000000001011101110101011001
    equals routeAC 0000000000000000000000000000000000110101010100011101110101011001
                                                                ^-- Overlap bit

cjdns nodes SHOULD use a `01` to reference themselves, therefor Bob's switch
parses `01` and sends the message to his own router. Then crafting a valid
answer to Alice's request for `routeBC` is a simple matter of taking Bob's route
to Charlie and flipping the least significant bit (the "overlap bit"). When
Alice gets the request, shifts and XORs, she will have a valid route.

Switches MUST NOT apply return path discriminators which are a different
length than destination discriminator. In order to add interfaces on the fly
without breaking existing routes, they may have discriminators of different
lengths but when handling a long destination discriminator from a short source
discriminator, the source discriminator MUST be represented using the same
number of bits. When sending responses to requests for routes, all responses
MUST be at least as long as the discriminator for the node who requested them.

Since label space is most efficiently used when a switch's largest
discriminator is closest to the same size as it's smallest discriminator,
renumbering interfaces is encouraged, especially right after start up when all
interfaces have just been registered. However, switches SHOULD NOT re-number
more than necessary as it breaks existing routes which run through them.

Packet priority part of an anti-flooding scheme which is based on the
observation that innocent traffic is generally balanced while attack traffic
is largely biased in one direction. Each switch SHOULD keep a running tab of
the total priority which has passed through each interface, increasing it when
packets come in and decreasing it when packets go out. If the total priority
exceeds a configured amount, switches SHOULD close the offending interface as
the other party is likely participating in a DDoS attack. Design and tuning of
this function is left up to the implementer, it is imaginable that the
priority fields could be used to promote fairness in peering among ISPs or for
other purposes but this is clearly a complex function and as such is being
left open to experimentation and discovery.

Inter-router control messages are [bEncoded][bEncode] [DHT][] messages which are used for
routers to exchange routes to other routers. The format used is borrowed from
[BitTorrent] with the following modifications:

Instead of "find_node" queries, nodes send "fr" queries (short for "find
router") "fr" responses are sent as a string called "rf" rather than "nodes".
"rf" strings are, as with nodes, concatenated ids and network addresses except
that the ids are 32 byte public keys and the network addresses are 8 byte
switch labels. The 32 byte key which is sent is not the DHT id, the key is
hashed using `sha512`, the output of which is `sha512` hashed again and the first
15 bytes of this become the node's id, these 15 bytes with a 0xFC byte
prepended also make up the node's ipv6 address. "get_peers" queries are not
used and the "id" value is never sent, node id is calculated as the double
sha512 of the node's public key.

................more to come

----

# Addenda

------------------- Some material to fit into the paper somewhere... -----------------

## Packet lifecycle:

RouterModule sends search,

SerializationModule bencodes the structures for the wire,

Ducttape.c takes the packet and gets the public key of the node which it is
   sending to and gets a CryptoAuth session associated with that key from it's
   pool, or creates one if there is none. Packet is cryptoauthed which adds a
   4 byte nonce header for all traffic packets or a 120 byte handshake header
   for the first 2 packets.

Ducttape takes the encrypted packet and applies the ipv6 header to it (if
   it was a data packet, the ipv6 header would have been copied out of the way
   before adding the cryptoauth header), the length of the ipv6 header is
   increased to take into account the cryptoauth header, (this is so the packet
   could theoretically be handled by commodity routing equipment).

SwitchConnectorModule gets a CryptoAuth session for the router it's sending to
   from it's pool, if there isn't any, it creates one. The packet is
   cryptoauthed, protecting the ipv6 header and adding another crypto header
   (again, 120 bytes unless the handshake is complete in which case it is 4).

Ducttape does not lookup the switch label to forward to because the router
   module already knows it so Ducttape uses what the router module proscribed.

The Switch sees an incoming packet and, not caring which interface it came
   from, sends it on to the interface which the bits dictate, shifting and
   applying the source interface as it is sent. When the packet exits through a
   UDP based interface, the UDPInterface chooses a CryptoAuth session which
   corrisponds to the destination IP address, this time the packet is encrypted
   *and* authenticated, adding either 20 bytes for traffic or 136 bytes for a
   handshake packet.

## CryptoAuth:

There are 5 types of CryptoAuth header:

1. "connect to me"
2. handshake1
3. handshake2
4. data
5. authenticatedData

All cryptoAuth headers are 120 bytes long except for the data header which is
4 bytes and the authenticatedData header which is 20 bytes. The first 4 bytes
of a CryptoAuth header is used to determine it's type, if they are zero, it is
a "connect to me" header, if they are equal to the obfuscated value of zero or
one, it is a handshake1 packet and if they are the obfuscated value of two or
three, it is a handshake2 packet. if it is the obfuscated value of a number
exceeding three, it is a data or authenticated data packet.

### 1) "connect to me" packets:

When a node receives a connect to me packet from a node which it does not
know, it should establish a session and send back a handshake1 packet, if it
already has a session, it should drop the packet silently. The connect to me
packet has no useful information except for it's system state and "Permanent
Public Key" field, the rest of the packet should be filled with random.

### 2) handshake1:

A handshake1 packet contains an authentication field, a random nonce, the
node's perminent public key, a poly1305 authenticator field, and the temporary
public key followed by the content, all encrypted and authenticated using the
perminent public keys of the two nodes and the random nonce contained in the
packet. The content and temporary key is encrypted using
crypto_box_curve25519poly1305xsalsa20() function.

### 3) handshake2:

A handshake2 packet likewise contains an authentication field, a random nonce,
a perminent public key field (which is not used but still must be present) a
poly1305 authenticator, and an encrypted temporary key and content. this time
the temporary key and content is encrypted using the perminent key of the
sending node and the temporary public key of the other party (which was sent
in the handshake1 packet).

### 4) Data packets


### 5) Authentication field:

This field allows a node to connect using a password or other shared secret,
the authtype subfield specifies how the password should be hashed, the auth-
derivations field specifies how many times the shared secret hash function
must be run and the authentication hash code is the 5 bytes of the sha256 of
the result from the hash function. Auth type of 0 indicates that the node is
not offering authentication credentials in the handshake, auth type 1 is a
trivial sha256 of the password to create the hash (TODO: improve this). When a
client presents authentication credencials, the result of doing the number of
derivations given on the password is then appended to the key generated by
point multiplication of the public and private keys and sha256'd to generate
the shared secret.

This authentication scheme is designed to be resistant to MiTM attacks as well
as attacks on the underlying asymmetric cryptography which protects the
connection. Basicly it is using a password as what it is, a shared secret. If
the auth type field is set to 0, the key generated by scalar multiplication
will not be fed to sha256, it will instead be hashed using hsalsa20 as
normally used in crypto_box_curve25519xsalsa20. If the A bit (at 14 byte
offset) is set, the connection will use poly1305 to authenticate all packets,
regardless of whether auth-type is 0.

# References

[1]: http://www.verisigninc.com/assets/whitepaper-ddos-threat-forrester.pdf "DDoS: A Threat You Can’t Afford To Ignore" 

[Bitcoin]: http://www.bitcoin.org/ "BitCoin: a decentralized electronic cash system using peer-to-peer networking, digital signatures and cryptographic proof to enable irreversible payments between parties without relying on trust."

[Namecoin]: http://dot-bit.org/Main_Page "Namecoin: a peer-to-peer generic name/value datastore system based on Bitcoin technology (a decentralized cryptocurrency)."

[IPSEC]: http://en.wikipedia.org/wiki/IPsec "IPsec: a protocol suite for securing Internet Protocol (IP) communications by authenticating and encrypting each IP packet of a communication session. IPsec also includes protocols for establishing mutual authentication between agents at the beginning of the session and negotiation of cryptographic keys to be used during the session."

[DNSSEC]: http://en.wikipedia.org/wiki/Domain_Name_System_Security_Extensions "A suite of Internet Engineering Task Force (IETF) specifications for securing certain kinds of information provided by the Domain Name System (DNS) as used on Internet Protocol (IP) networks. It is a set of extensions to DNS which provide to DNS clients (resolvers) origin authentication of DNS data, authenticated denial of existence, and data integrity, but not availability or confidentiality."

[DNS]: https://en.wikipedia.org/wiki/Domain_Name_System "A hierarchical distributed naming system for computers, services, or any resource connected to the Internet or a private network. It associates various information with domain names assigned to each of the participating entities. Most importantly, it translates domain names meaningful to humans into the numerical identifiers associated with networking equipment for the purpose of locating and addressing these devices worldwide."

[P2P]: http://en.wikipedia.org/wiki/Peer-to-peer "Peer-to-peer (P2P) computing or networking is a distributed application architecture that partitions tasks or workloads among peers. Peers are equally privileged, equipotent participants in the application. They are said to form a peer-to-peer network of nodes."

[Internet]: http://en.wikipedia.org/wiki/Internet "A global system of interconnected computer networks that use the standard Internet protocol suite (TCP/IP) to serve billions of users worldwide. It is a network of networks that consists of millions of private, public, academic, business, and government networks, of local to global scope, that are linked by a broad array of electronic, wireless and optical networking technologies."

[DDoS]: http://en.wikipedia.org/wiki/Denial-of-service_attack "An attempt to make a computer or network resource unavailable to its intended users. Although the means to carry out, motives for, and targets of a DoS attack may vary, it generally consists of the concerted efforts of a person, or multiple people to prevent an Internet site or service from functioning efficiently or at all, temporarily or indefinitely."

[bEncode]: http://en.wikipedia.org/wiki/Bencode "The encoding used by the peer-to-peer file sharing system BitTorrent for storing and transmitting loosely structured data."

[DHT]: http://en.wikipedia.org/wiki/Distributed_hash_table "A class of a decentralized distributed system that provides a lookup service similar to a hash table; (key, value) pairs are stored in a DHT, and any participating node can efficiently retrieve the value associated with a given key. Responsibility for maintaining the mapping from keys to values is distributed among the nodes, in such a way that a change in the set of participants causes a minimal amount of disruption. This allows a DHT to scale to extremely large numbers of nodes and to handle continual node arrivals, departures, and failures."

[BitTorrent]: http://en.wikipedia.org/wiki/BitTorrent_(protocol) "A peer-to-peer file sharing protocol used for distributing large amounts of data over the Internet. "

[Tor]: https://www.torproject.org/ "Tor is free software and an open network that helps you defend against a form of network surveillance that threatens personal freedom and privacy, confidential business activities and relationships, and state security known as traffic analysis."

[I2P]: http://www.i2p2.de/ "An anonymizing network, offering a simple layer that identity-sensitive applications can use to securely communicate. All data is wrapped with several layers of encryption, and the network is both distributed and dynamic, with no trusted parties."

[Freenet]: https://freenetproject.org/ "Free software which lets you anonymously share files, browse and publish "freesites" (web sites accessible only through Freenet) and chat on forums, without fear of censorship. Freenet is decentralised to make it less vulnerable to attack, and if used in "darknet" mode, where users only connect to their friends, is very difficult to detect."

[Phantom]: http://en.wikipedia.org/wiki/Phantom_Anonymity_Protocol "A system for decentralized anonymization of generic network traffic."