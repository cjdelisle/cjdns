# How does cjdns handle duplicate nodes?

`cow_2001` asks:

> what happens if there are ip clones?
>
> does the network knows how to handle that?

## There are two ways that this can play out:

1. At least two nodes with identical ipv6 addresses, but different keys.
2. Two instances of the same public-private keypair (and necessarily the same ipv6) connected to the same network from different locations.

## Scenario one

As addressed by `gloe-ih`:

> the address is in the range of a hash function and, by definition, multiple keys can have the same fingerprint, however you'll not be able to establish a cryptoauth session with that node with the knowledge of another key
> because you know that key1 -> addr, so you'll try to use key1 as the pubkey for that session, and the node owning key2, with the same addr, won't be able to decrypt

So the session _should_ be rejected, but in reality this is incredibly hard to test. As far as we know, nobody has generated to distinct private keys with identical ipv6 addresses. The probability of someone doing so is incredibly low..

> <@ircerr> ? lotto
>
> <@irbawt> lotto: 1 in 1,329,227,995,784,915,872,903,807,060,280,344,576 chance of generating the same IPv6. Feeling Lucky?

## Scenario two

`cow_2001` asks:

> won't the routing be all messed up because routing is address dependant?

This hasn't really been tested, or, if it has been, nobody has reported their findings. Everything below is speculative, so if you find the answer, feel free to update this document.

We expect that if two distinct nodes (using different ipv4 addresses if connecting via udp) cannot successfully connect to a common peer directly using the same configuration file. The common node won't know which connection to treat as the actual destination.

If two nodes are launched, and they connect to different parts of the network using the same configuration file, then in theory they may both function. Since indirect connections are made using only a virtual address (your cjdns ipv6), the two nodes should be indistinguishable to anyone trying to connect to them.

Cjdns works by using the first path it finds, then replacing that path if it finds a better one. It is commonly believed that if a node were to try to connect to that ipv6, they would first try to find a path to that node, then they would establish a cryptauth session. Whichever instance of a node it found first would establish this session. 

It's possible that the connecting node might find a path to each target node, but it wouldn't know the difference between them aside from the fact that it had established a cryptauth session with one but not the other. We are unsure of how it would handle that information, however, it might prevent the connecting node from flipping between two distinct sessions. This would be desirable, since doing so would probably wreak havoc on the higher level protocols being established over such a connection.

If that's the case, then this could be used to provide an inherent [load balancing](http://en.wikipedia.org/wiki/Load_balancing_%28computing%29) effect.

As noted above, this has not been thoroughly tested. Changes to the source code in the future could possibly make this impossible (via assertion failures). If it is currently a _property_ of cjdns, then the best way to ensure that it continues to function would be if someone were to take advantage of it, and report its breakage with future revisions.

So please, give it a try, and let us know what you experience. Write about how you used this to your advantage, and go down in cjdns history!


09:55 < cow_2001> gloe-ih: yes
09:55 < gloe-ih> check if it bounds to the tun interface
09:56 < cow_2001> it has a build plan for x86_XVA or w/e it's called
09:56 < gloe-ih> (when running as root by default it spawns it)
09:56 < cow_2001> i don't want to run as root
09:56 < cow_2001> too scary
09:56 < gloe-ih> so follow the doc ;)
09:56 < cow_2001> i'm reading on configuration of the cjdroute.conf
09:57 < cow_2001> i've set up a tun thing for the cjdns user
09:57 < gloe-ih> you'll have to add addresses / routes manually


