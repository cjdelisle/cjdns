## The Secret Santa Scenario

This is a possible routing issue that may be related to the **black hole routing bug**. I (ansuz) decided to give it a distinct name because it may be an entirely different problem with the routing behaviour. More distinctive names are more likely to get remembered, and ∴ they are more likely to be _reported_. At least, that was my reasoning.

## The Expectation

**cjdns** distributes a network's routing table across its constituent nodes. Individual nodes decide which other nodes they should keep track of by a combination of two main metrics:

+ a **logical** proximity metric (in which nodes which are a small number of hops away are considered close)
+ a **key-wise** proximity metric (in which nodes with similar addresses (by a xor metric) are considered close)

The idea is that nodes should know which peers are logically proximal, for obvious reasons, but they should also track a number of distant peers which are chosen randomly. Rather than using continuous random functions, nodes capitalize on the assumption that private-key generation (upon which each node's ipv6 address is dependent) should be random. As such, we expect ipv6 addresses to be distributed without any predictable correlation to physical proximity.

If a node **A** goes looking for a node **M**, **A** first checks if **M** is a logically proximal peer, then falls back to asking peers with similar keys if they know of a path to **M**. We expect that any node should be able to find any other node in `log(n)` lookups, where `n` is equal to the size of the network.

## The problem

This method of distributing paths among nodes (aka constructing a Distributed Hash Table (**DHT**)), is based upon **[Kademlia](http://en.wikipedia.org/wiki/Kademlia)**. There may be a problem with this (this is the part where I bring Santa into it).

Many of the world's nations have a holiday practice sometimes referred to as a [Secret Santa gift exchange](http://en.wikipedia.org/wiki/Secret_Santa). The premise is that some group of people (often the employees of some organization) each put their name into a receptacle. Once each member has inserted their name, they take turns removing a name (drawing again if they draw their own name). Once every member has received a name, they give gifts to their random recipient. When it goes well, everyone receives a gift.

The assumption is that even if they do not know their chosen recipient well, they have enough common acquaintances that they can learn enough about them to deliver a suitable gift. Furthermore, it should go without saying that they should have the capacity to deliver this gift. The Kademlia method is generally implemented as a dynamic overlay network. Its member are assumed to have a path to one another.

This is not the case with **cjdns**, as it is intended to be used to form a **[pseudonymous](http://en.wikipedia.org/wiki/Pseudonymity)** network. In other words, it is not guaranteed that there already exists a path to the their target node (or gift recipient, to return to the metaphor).

## Relying on chaos

Suppose I need to find a node: I ask my peers, they ask their peers, and so on. Since their is not necessarily a common mode of communication shared between all nodes, there are edge cases in which we will never find the node in question. Despite the assurances that our random distribution **should** be even, it is possible that we are only able to see four hops away, and the nearest node which happens to have a path to the target node is 7 hops away. Nodes which are three hops from us **should** be able to see as far as we are able to (four hops), but in reality the directions in which they are able to see may not be in the right direction. They may be able to see further into one part of the network than another (like a car's blind spot, or in the eye of an individual afflicted with cataracts).

To put this in human terms: 

* Our work is organizing a secret santa gift exchange.
* We work for a multinational corporation.
* We receive a name we aren't familiar with.
* We ask around, nobody knows the individual in question.
* We ask the human resources department, and they inform us they can't give away that information (it's private).
* That person never gets a gift.
* This happens through the company, and lots of people don't get gifts.
* Bummer.
