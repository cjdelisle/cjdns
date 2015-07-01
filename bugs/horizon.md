One of the central ideas behind the development of cjdns is that networking protocols should scale up smoothly.
Early development dealt with problems like **not** constructing circular routes, and while that was not yet solved it made sense to limit the length of a path to what would fit within 64 bits of memory.

At this point, many of the early issues that were once challenging have been dealt with, and tucked away into a list of things that we shouldn't forget, but which are no longer relevant.
As the protocol has improved and allowed nodes to see and route further into the network, though, we have begun to encounter bugs which we previously had no opportunity to debug, and consequentially, no opportunity to fix.

The **Horizon** bug is one that we knew was coming, but now we've officially started to hit its limitation.

## The nature of the bug

* nodes have 64 bits of memory in which they can store a path to another node
* each node along the path consumes a variable number of bits dependiung on the number of peers between which they must distinguish (see [switchfun.txt](../switchfun.txt) for some idea of how this works)
* your node cannot route to any node beyond this _horizon_ for the lack of bits required

## So what are the implications of this bug?

1. nodes with more peers consume more bits, and are generally a good thing as they reduce the number of hops to a wider audience of nodes
2. nodes with fewer peers consume fewer bits, and result in more hops on average, however, their simpler encoding will allow your node to reach further into the network
3. If you want to have effective routing to the greatest number of nodes possible, the onus is on you to evaluate your position in the network and peer in such a way that the nodes you wish to connect to are within a reasonable distance (via 'backhaul' links with few peers, or via hubs which connect you within a few hops to a large portion of the net)

## How can we deal with this?

Naturally, we can approach the problem from a few angles, so whatever your skill set is, there's likely something that you can do to help.

1. help rewrite the parts of cjdns which encode the paths such that a variable number of bits can be used
2. use the admin interface to figure out which parts of the network your node can see, and compare those results against a wider view of the network, so we can identify our blind spots
3. evaluate your peering choices, and try to drop bad peers and pick good ones, so that we can continue to have a functional network in the meantime (we're still quite likely to have lots of paths that are out of reach, so we'll still be able to debug this issue, don't worry)
4. talk to your friends about cjdns, and get them to join the network, so we have more opportunity to find long _shortest-paths_ to any particular node (see, you don't even need to know how to code!)

## Conclusion

This is great news! That we are hitting this bug at all means we've progressed quite a bit.
Let's sort this one out so we can move on to the next and make cjdns the de facto choice for mesh networking!
