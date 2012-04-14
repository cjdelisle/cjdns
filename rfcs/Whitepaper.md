*This is a work in progress*

# cjd's Networking Suite -- Everyone Still Thinks It's A Nameserver

## What?

Imagine an Internet where every packet is cryptographically
protected from source to destination against espionage and forgery, getting
an IP address is as simple as generating a cryptographic key, core routers
move data without a single memory look up, and denail of service is a term
read about in history books. Finally, becoming an ISP is no longer confined
to the mighty telecoms, anyone can do it by running some wires or turning on
a wireless device.

This is the vision of cjdns. 

## Why?

The Internet is built on protocols which largely date back to the late 80's
or earlier. At a time when it was a network of anarchistic academics and
scholars showing the ITU that open standards matter, it was absolutely enough.
Over time the network has gotten bigger and the users have found new needs.

In the age when packet inspection is universal and security breaches are
commonplace, cryptographic integrity and confidentiality are becoming more of
a requirement. The US government recognized this requirement and has been
helping through [IPSEC] and [DNSSEC] efforts.

Another issue is how are we going to route packets in a world where the global
routing table is simply too large for any one router to hold it all? Despite
the heroic efforts of core network engineers, the growth of the global routing
table seems an unstoppable march. Cisco router company has proposed a plan
called Locater/Identifier Separation Protocol, or [LISP] which aims to solve
this by re-aggregating the routing table without forcing people to change their
precious IP addresses. A different view of this problem is IP address
allocation, currently it is done by a central organization which assigns IP
addresses in such a way as to make the routing table as small as possible.
Unfortunately this creates a bar of entry to the ISP sphere because aspiring
network operators must register with the central organization and apply for an
allocation of IP addresses while demonstrating that they will not be wasted.
It is always easier to show that you need IP addresses if you already have a
network.

Denial of service, an attempt to prevent legitimate users from accessing a
service[1], is likewise a new problem in the expanding network. To my knowledge,
there is no general purpose solution to denial of service attacks. Solutions to
packet flood based denial of service often revolve around hosting a service on
many computers so that they can handle an enormous amount of traffic.

Finally, the existing protocols are difficult to use, we cannot reasonably
assign blame to anyone for this, many of these protocols are over thirty years
old and demonstrate a level of craftsmanship which I can only hope to one day
achieve. However, thirty years takes it's toll on the best of us and as the
Internet grew and became more complex, the administration interface of the
typical router has grown a thicket of knobs, buttons and switches to match the
proliferation of use cases and failure modes. As a result, network operation
has become a science where students receive degrees and certificates for knowing
the meanings of the plethora of knobs and switches, it has also become, like the
tuning of the race car, an art, passed from master to apprentice and shared on
mailing lists. Suffice to say, the bar of entry into the ISP realm is too high.
Users, particularly in the ad-hawk wireless network arena have observed the high
bar of entry into traditional routing and have developed a menu of alternative,
self-configuring protocols such as [OSLR], [HSLS], and [BATMAN].


### So the problems are already solved?

Not every problem listed has an existing solution and of the ones which do,
many of the solutions are based on incompatible technology. For example: OSLR
was not designed to operate with IPSEC and LISP. Even where the solutions exist
and are ready for deployment, they still require mass technology adoption and
they don't offer existing ISPs significant immediate gains.

The mismatch is rather absurd. On the one hand there are scholars, engineers,
hardware and software designers with combined over 1000 years of experience.
There are mathematical formulae, proofs, papers and specification documents;
written, circulated, peer reviewed and written again. On the other hand you
have a single volunteer developer, a clean slate, and an attitude that
nothing is impossible. How can this be anything short of lunacy?

*In revolutionary times, the old book only weighs you down.*

cjdns is built on the idea that the ISPs and hosting providers which exist
now will never upgrade, not to LISP, not to DNSSEC, not even to IPv6 in any
meaningful way. Building new systems to be compatible with old systems is
catering to the audience you will never have. Asking existing ISPs to upgrade
for the common good is asking them to take a risk with no immediate benefit.
cjdns is about throwing out the book and redefining the specifications in a
way that will be fast, secure, and most importantly, *easy* for the next
generation of ISPs to deploy and use.


### What is this denial of service?

Usage of a service can be interrupted by sending a flood of unwanted packets
to a host from a large number of infected "zombie" machines. This, known as
[DDoS], is a problem which worsens every year as the upload speed of all
infected nodes on the Internet grows in proportion to the download speed of any
given link. Being infected with a virus and participating in DDoS, though not a
picnic, is not an emergency for the owner of the infected machine. Nor is it an
emergency for their ISP. DDoS is always "their problem"... Until it strikes in
your network. Sadly, a common response from a datacenter is to stop carrying the
controversial content, making DDoS an effective censorship tool and encouraging
the practice.

Another form of denial of service which is even more insidious is intimidation
by threat of faux court action. This form of denial of service is especially
effective since most people get their IP addresses from their ISP, when their
ISP is threatened, they need to make a judgment call as to the validity of the
claim and they often act as judge an jury, turning off a customer in order to
avoid conflict. Those who have their own IP addresses assigned to them, are able
to essentially be their own ISP and to peer with a multitude of other ISPs
making it very difficult to threaten anyone but them.


### What is the routing table and why does it keep getting bigger?

A more technical issue with the Internet, and one of which many people are
unaware, is address space deaggregation. Every computer connected to the
Internet needs an address, a number which uniquely identifies it and which is
attached to every piece of data which is to be sent to that computer. At every
stop along it's path through the Internet, a packet (unit of data) has it's
address field examined by a router so it can decide which wire that packet
should be sent down. Routers have an easier time if addresses are in big blocks
so that a router can look quickly at the first numbers in the address and know,
for example, that it is destined for somewhere in China, not exact but enough to
know which wire to send it though. People naturally want as many addresses as
they can possibly get and they want them in the smallest blocks possible, this
is so they can then control (or buy and sell) these small blocks independently.
The smaller the blocks of addresses which are announced, the larger the routing
tables become and the more work the Internet's core routers must do in order to
send a packet in the right direction. There have been attempts to aggregate
addresses back in to groups but nonetheless, the number of small announcements
in the global routing table has grown every year.

To address the growing routing tables, Cisco has proposed a new protocol called
Locator/Identifier Separation Protocol or LISP. The idea of LISP is to
separate the addresses which people use from the addresses which routers use,
like a lower level version of DNS. LISP allows the edge ISPs and users to see
the Internet as they want it, deaggregated into small pieces for political
reasons, and the routers to see the Internet as they want it, centralized
hierarchical pyramid of addresses emanating from some arbitrary center point.
This design works well in existing routers since they are designed to get
packets with a universally unique address and look that address up in a table.
This is advertised as a design feature but LISP is limited in it's vision, if
one must look up the "real location" of a server before forwarding a packet,
why not simply look up the fastest path?


### I don't care, it's their problem.

Each of these problems is a tragedy of the commons problem. The users of virus
infected computers are incentivized to save money rather than purchasing a
product or service to rid their computer of the infection. While there are
solutions such as egress filtering which decrease the problem, ISPs are
incentivized to implement as little security as possible because they are not
directly affected. Denial of service whether by packet flooding or by faux legal
action benefits the attacker who is able to hide the truth or victimize service
providers as well as organizations who make it their business to provide DoS
related services. The victim who all too often publishes information for no
other reason than satisfaction of telling the truth, is the only party harmed
by this type of attack, and he is the least able to prevent it. Address space
deaggregation benefits the edge ISPs who gain more flexibility in how their
network is organized at the cost of the core ISPs whose only defense is the
"we will not route that" nuclear option which would no doubt bring about a
revolt from the edge ISPs. Each of these problems hurts everyone, DDoS forces
ISPs to over prevision their lines, denial of service through faux legal action
increases the cost of running a community website or ISP since each accusation
must be reviewed and it's validity assessed, and address deaggregation means
everyone must pay more to have their packets routed through increasingly high
power routers, and difficulty of operating a router and getting a block of IP
addresses hurts competition in the ISP sphere, thus increasing prices and
impeding progress.


## How?

cjdns is made of three major components which are woven together.
There is a switch, a router, and a CryptoAuth module. With total disregard for
the OSI layers, each module is inherently dependent on both of the others. The
router cannot function without routing in a small world which is made possible
by the switch, the switch is blind and dumb without the router to command it,
and without the router and switch, the CryptoAuth has nothing to protect.


## The Switch

Switches use an internal numeric compression scheme to compress the interface
index into a few bits of the 64-bit label. How they compress the number is an
implementation detail as long as they can read a label number and know how many
of the low bits "belong to them" as opposed to the next switch along the path.
After determining the correct destination interface, the switch will bit shift
the label to the right and add reversed bits to the now empty left side of the
label such that if the entire label were reversed, the switch would send the
packet in the opposite direction. In the event of an error, the switch does a
bitwise reversal of the entire label and sends the packet back where it came
from. By doing a full bitwise reversal, the switches need not care how other
switches encode the numbers or be able to reverse the order since they can
reverse the the entire label.

                        1               2               3
        0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     0 |                                                               |
       -                         Switch Label                          -
     4 |                                                               |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     8 |      Type     |                  Priority                     |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

Switch headers are designed to be small and efficient. The fields include the
`label` of which some number of bits (henceforth known as a discriminator)
belong to each switch along the forwarding path, the `Type` field indicating
the type of packet. Reserved packet types are `0` for opaque data, and `1` for
switch control messages (eg errors). The `Priority` field contains a number
which represents how important the delivery of a packet is. When a link becomes
saturated, the switch sending packets over that link SHOULD drop packets of
least priority and MAY decrease the priority of all packets passing through it.
When packets are dropped, switches SHOULD emit an error packet with the inverse
label to be sent to the sender. Switches SHOULD make adjustments based on error
packets which are sent in response to packets which they forwarded, forwarding
error packets is OPTIONAL, in flood situations it may not be wise.

If Alice wants to send a packet to Fred via Bob, Charlie, Dave and Elinor, she
will send a message over her interface to Bob's. This packet will have a label
that causes the packet to be routed to Charlie then on to Dave.

NOTE: Spaces between bits are for illustration only, switches do not know how
many bits of a label are used by anyone other than themselves.

Alice's original label:

    00000000000000000000000000000000000000 101011 011010 100101101 10111
    ^^^^^^^^^-- unused space --^^^^^^^^^^                          ^^^^^-- Bob's discriminator for routing to Charlie.

Bob shifts off his discriminator and applies to the top of the label the bit-
reversed discriminator for the Bob->Alice interface.

    11001 00000000000000000000000000000000000000 101011 011010 100101101
    ^^^^^-- Bob's discriminator for Alice, bit-reversed.       ^^^^^^^^^-- Charlie's discriminator for Dave.

Charlie removes his discriminator and applies the reversed discriminator for
sending to Bob then forwards to Dave.

    110110011 11001 00000000000000000000000000000000000000 101011 011010
    ^^^^^^^^^ ^^^^^-- Bob's discriminator for Alice (reversed).   ^^^^^^-- Dave's discriminator for Elinor.
            ^-- Charlie discriminator for Bob (reversed).

Supposing Dave cannot forward the packet and needs to send an error, he does
not know where Charlie's discriminator ends and Bob's begins so he can't
re-order them but because they are bit reversed, he can reverse the order by
bit reversing the entire label.

    010110 110101 00000000000000000000000000000000000000 10011 110011011
                                                         ^^^^^ ^^^^^^^^^-- Charlie's discriminator for Bob.
                                                         ^-- Bob's discriminator for Alice.

Dave can then send the packet back to Charlie who need not know what it is in
order to forward it correctly on to Bob and then to Alice. If the packet had
reached Fred, he would be able to use the same technique of reversing the label
in order to determine it's origin.

In order for labels to be able to be spliced together, the most significant bit
in a label must always be `1` so that we know where it ends. Since all routes
must end at a router, this means that all switches must regard `1` as a
reference to the router which sits atop them. Specifically, any label whose
least significant 4 bits are `0001` MUST be regarded as a self reference and if
splicing two labels together results in less than 3 `0`s between the highest set
bit and the beginning of the label, the splice has failed. This is important so
that the reverse route data applied by routers along the path is not mistaken
for additional forward route.

Splicing is done by XORing the second part with `1` and shifting it left by the
log base 2 of the first part, then XORing the result with the first part. 

Given:

    routeAB =        0000000000000000000000000000000000000000000001011101110101011001

    routeBC =        0000000000000000000000000000000000000000000000000000110101010100
    XOR 1            0000000000000000000000000000000000000000000000000000000000000001
    equals           0000000000000000000000000000000000000000000000000000110101010101

    << log2(routeAB) 0000000000000000000000000000000000110101010101000000000000000000
    XOR  routeAB     0000000000000000000000000000000000000000000001011101110101011001
    equals routeAC   0000000000000000000000000000000000110101010100011101110101011001
                                                                  ^-- Overlap bit

The log base 2 represents the index of the first set bit, starting from the
right. This means that shifting by the log base 2 leaves 1 bit of overlap, this
along with the XORing of the second part (`routeBC`) against `1` causes the
highest bit in the first part to be overwritten.

In order to allow a switch to add more interfaces without knowing how many it
will use in advance, switches should be able to add new interfaces whose
discriminators use more bits than the ones for the old interfaces. However, when
a switch forwards a packet, the source discriminator MUST NOT be longer than the
destination discriminator, otherwise there would not be be enough room for it in
the space made by shifting the label. To resolve this, a switch's number
compression scheme MUST allow it to represent all discriminators shorter than X
bits, using X bits. Routers MUST always return routes using at least as many
bits in the first discriminator as are used for the discriminator for the node
who is asking. Finally, switches MUST drop packets for which the discriminator
is represented in fewer bits than the smallest representation of the source
interface for the packet. Switches SHOULD send back an error packet so that the
bogus route may be purged as soon as possible.

A packet whose label requests that it be routed back down the same interface from
which it came SHOULD be dropped and an error packet SHOULD be sent back so that
the "redundant route" may be resolved.

Since label space is most efficiently used when a switch's largest
discriminator is closest in size to it's smallest discriminator, renumbering
interfaces is encouraged, especially right after start up when all interfaces have
just been registered. However, switches SHOULD NOT re-number more than necessary as
it breaks existing routes which run through them.


## The Router

A router has 3 functions, it periodically searches for things, responds to
searches, and forwards packets. When a router responds to a search, it responds
with nodes which it thinks will get closer to the destination. The responses
MUST NOT have addresses which are, in address space distance, further from the
responding node than the search target, and they MUST NOT have routes which
begin with the same interface as the route to the querying node. These two
simple rules provide that no search will ever go in circles and no route will
ever go down an interface, only to be bounced back. While the second rule can
only be enforced by the honer system, querying nodes MUST double check the first
rule. The node doing the searching adds the newly discovered nodes to their
routing table and to the search, then continues the search by asking them.

Upon receiving a search response containing one's own address, a node SHOULD
purge all entries from it's table whose routes begin with that route. This will
control the proliferation of redundant routes.

The "address space distance" between any two given addresses is defined as the
of the result of the two addresses XOR'd against one another, rotated 64 bits,
then interpreted as a big endian integer. The so called "XOR metric" was
pioneered in the work on [Kademlia] DHT system and is used to forward a packet
to someone who probably knows the whole route to the destination. The 64 bit
rotation of the result is used to improve performance where the first bits of
the address is fixed to avoid collisions in the IPv6 space.

Adding nodes to the routing table from search responses is done by splicing the
route to the node which was asked with the route to the node in the response,
yielding a route to the final destination through them.

Router messages are sent as normal UDP/IPv6 packets except that their UDP source
and destination port numbers are zero and the hop limit (TTL) field in the IPv6
header is set to zero. Any packet which meets these characteristics is to be
considered a router message and any packet which doesn't is not.

The content of the inter-router messages is [bEncoded][bEncode] dictionaries.
Routers send search queries which have a key called "q", and replies which
don't. Routers SHOULD reply to a message with a "q" entry but MUST NOT reply if
there is none, lest they reply to a reply. All messages have a transaction id
number, a sort of cookie made of a bencoded string containing arbitrary bytes
which must be reflected back in the reply. The most common query is a find node
or "fn" query. "fn" queries have a field called "tar" for the target address
which the node is looking for. Responses to "fn" queries have a field called "n"
which is a binary string containing the 32 byte public keys and 8 byte switch
labels for the responses.

Example fn query in JSON:
    {
        "q":    "fn",
        "tar":  "abcdefghhijklmno",
        "txid": "12345"
    }

Same query bEncoded as the routers use:
    d1:q2:fn3:tar16:abcdefghhijklmno4:txid5:12345e

Example fn reply in JSON:
NOTE: this reply only shows 2 nodes returned and is for illustration purposes
in most cases the number would be an implementation specific constant around 8.
    {
        "n": "cdefghijklmnopqrstuvwxyzabcdefghi1234567qponmlkjihgzyxwvutsrstuvwxyzabcde2345678"
        "txid": "12345"
    }

Same reply bEncoded
    d1:n80:cdefghijklmnopqrstuvwxyzabcdefghi1234567qponmlkjihgzyxwvutsrstuvwxyzabcde2345678e


The nodes in an fn reply are ordered from worst to best so the best answer is
the last entry in the reply.

Routers choose the node to forward a packet to in a similar way to how they
answer search queries. They select nodes from their of their routing table
except in this case the selection contains only a single node.


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

[OSLR]: http://tools.ietf.org/html/rfc3626

[HSLS]: http://www.ir.bbn.com/documents/techmemos/TM1301.pdf

[BATMAN]: http://en.wikipedia.org/wiki/B.A.T.M.A.N.

[1]: http://www.cert.org/tech_tips/denial_of_service.html

[2]: http://www.verisigninc.com/assets/whitepaper-ddos-threat-forrester.pdf "DDoS: A Threat You Can’t Afford To Ignore" 

[LISP]: http://lisp.cisco.com/

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

[Kademlia]: http://pdos.csail.mit.edu/~petar/papers/maymounkov-kademlia-lncs.pdf

[Tor]: https://www.torproject.org/ "Tor is free software and an open network that helps you defend against a form of network surveillance that threatens personal freedom and privacy, confidential business activities and relationships, and state security known as traffic analysis."

[I2P]: http://www.i2p2.de/ "An anonymizing network, offering a simple layer that identity-sensitive applications can use to securely communicate. All data is wrapped with several layers of encryption, and the network is both distributed and dynamic, with no trusted parties."

[Freenet]: https://freenetproject.org/ "Free software which lets you anonymously share files, browse and publish "freesites" (web sites accessible only through Freenet) and chat on forums, without fear of censorship. Freenet is decentralised to make it less vulnerable to attack, and if used in "darknet" mode, where users only connect to their friends, is very difficult to detect."

[Phantom]: http://en.wikipedia.org/wiki/Phantom_Anonymity_Protocol "A system for decentralized anonymization of generic network traffic."
