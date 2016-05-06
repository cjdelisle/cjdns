# Project goals

## With built-in security and auto-configuration, everybody can own a part of the network

The Internet gives everyone incredible power of expression that was once
reserved for those wealthy enough to own a radio station or newspaper. Still,
the Internet's aging protocols have inherent limitations which make them
unfavorable toward a network owned by the people.

Recent revelations over governments reading our emails have triggered public
outcry but few stop to imagine the implications of an unsecured
mesh network, in such a network *everybody* could read your email.

Competition in Internet Access markets worldwide has withered, dominated by a
few businesses who charge outrageous rates and have not materially increased
Internet speed since crushing the dial-up providers. Most agree that more market
diversity is necessary, but if the handful of ISPs we have now cannot be trusted
to handle our data, there is no reason to expect that the next generation of
Internet Service Providers would either.

The problem of trust extends beyond private correspondence. The very
fabric of the Internet can be torn apart by a malicious ISP or even an honest
mistake. On April 8th, 2010, an employee at China Telecom misconfigured a
router - causing widespread Internet outages lasting up to fifteen minutes.

As world governments continue pushing to filter websites, there is an ever
increasing risk of back-and-forth retaliatory filtration eventually leaving
entire nations isolated and breeding the hate and intolerance which the Internet
promised to end.

Cjdns was designed with the understanding that for the Internet to continue
existing without borders, authority over its processes must be decentralized.
This isn't, and can't be, just a dream - the future of the open Internet is at
stake.


## Security

When you receive a packet of information from the Internet, it seems logical to
assume that it was meant for you - that it came from the computer which it says
it came from and that nobody else has read or modified it on the way. While many
popular software applications are designed around these assumptions, the
existing Internet does not guarantee any of them and a number of network
security exploits come from the cases where these assumptions break down.

Cjdns guarantees confidentiality, authenticity and integrity of data by using
modern cryptography in a non-intrusive way. Information transmitted over a cjdns
network can't be altered or read en-route. While you can create multiple
identities, it's practically impossible to impersonate other nodes on the
network and since a node's IPv6 address is the fingerprint of its key,
man-in-the-middle attacks are not possible.


## Simplicity

Traditional networks require manual configuration of IP addresses. For one to
get these addresses one must join an Internet Registry and file a lengthy
application. Cjdns nodes generate their own addresses along with their keys.
When two nodes find each other, they connect. When many nodes find one another,
they form a network. General network architecture is of course needed to avoid
bottlenecks but once the nodes are put in the right places, they will discover
their roles in the network.


## Scalability

Cjdns is built around the bold and unproven assumption that a non-hierarchical
network can scale. Cjdns uses a [distributed hash table][] to spread the load
of routing among a number of nodes, rather than requiring every node know the
exact location of every other node. At the bottom layer, packets are tagged
with the exact route they should take, think of it like driving directions.
At the upper layer the nodes maintain and test routes to other nodes who have
numerically similar IPv6 addresses to their own. Forwarding is achieved by
sending a packet to physically nearby nodes who have destinations numerically
close to the target address.
