# Random details of cjdns' inner workings

ansuz' notes of a Q&A with Arceliar

## Probability of IPv6 address collisions

Or the math behind the birthday problem RE: cjdns

*Ansuz:* I remember that it was something like 1/10^18, and I remember the basic expression to derive that, but you had some fancy trick that got around the combinatorial explosion

*Arc:* Do you want the number of nodes before there's a >50% probability of a match? 1%? 0.1%?... I know there's a general formula for this, or at least a damn good approximation of it. Give me a min...

*Ansuz:* awesome, yea, maybe the method of how you calculate it efficiently. Maybe I could put it into javascript on a webpage so people can play with those values?

*Arc:* Found it: (looks like it's also on wikipedia!)

- n = number of nodes needed for this set of probability and keyspace size. Note that this is the - nodes needed for there to be *any* match *anywhere*... *your* node is almost always afe-
- p = probability of a match-
- d = number of possible add- resses (keyspace size)-
- ~= = approximately equal to (for sufficiently large values of d, which is definitely not a - problem)
- n ~= sqrt(2d * ln(1/(1-p)))

And some specific numbers for cjdns: (d = 2^120, because 8 bits are taken up by the leading fc. Note that the range of a sha512sum might not cover the full keyspace for all we know, though I would be very surprised if that were the case... but it's technically an open problem AFAIK)

- n(0.1%) = 5.15731 * 10^16
- n(1%)   = 1.63458 * 10^17
- n(50%) = 1.35746 * 10^18 ('birthday problem' answer)

If you want to see a plot of number of guesses needed (y-axis) to have a probability (x-axis) of there being at least one collission in the network, then punch this into wolfram alpha:

- y = sqrt(2*(2^120)*ln(1/(1-x))) from 0,1
- *Ansuz:* I'm assuming 1 == 100%, right? (On the x-axis)
- *Arc:* Yes, it does

> \<ansuz> +lotto
> \<mg2bot> 1 in 1,329,227,995,784,915,872,903,807,060,280,344,576 chance of generating the same IPv6. Feeling Lucky?

*Arc:* And that gets you the chances of anyone guessing a key that gets them *your* IP. Even if they do, that doesn't mean its your key, so your traffic is still safe... it just means they can impersonate you (which would probably cause crazy things to happen to the routing logic, we should give two peers the same key just to test that...)

Ansuz: A common problem I've noticed is that we store our information on IRC  bots that sometimes die. and a snippet like +lotto doesn't really  explain anything.
It's just a huge issue that these things get figured out, and then forgotten. and people are supposed to just trust some irc bot that everything is secure.
I'm trying to gather up all this info and put it in a better format, and mirror it all over the place (sync'ed with rsync/git atm)

## About XOR distance and finding nodes

Ansuz: I've heard that cjdroute remembers nodes with similar xor values. randati is using this for fc00.org - generating new nodes to help expand his routing table.

Arc: At least on crashey, it's probably easier to make the size of the node store larger in dht/dhtcore/NodeStore.h -- just add a 0 to the nodestore and linkstore sizes at the top. I have done both to find as many nodes as possible

there's a patch set that will probably be pulled in a few days, which changes the way crashey decides which nodes to search for. in theory, giving it a nodestore size larger than the network should eventually find every node...

Ansuz: so I'm curious if this is true, and maybe if we're making a meshlocal people will be able to route better if they have a closer address?

Arc: I'm not sure how this affects (if it does at all) who we should be peering with...
Speaking about crashey, as things have changed quite a bit from how they were done in the old pathfinder... (old pathfinder found routes at random, stored ~all of them. in theory it had to discard them somehow if it was full but i don't know how, as i never got close to full...)
Nodes make it into our routing table one of two ways. Both are steered from dht/dhtcore/Janitor.c for the most part.

1. When we discover a new node (direct peer, mentioned in a search result, etc) we add the path to them to a list of possible nodes called the RumorMill. Once per second, we send a getPeers request to a node from the RumorMill. If the mill is full, we discard the 'worst' one, where badness is defined mostly in terms of how far they are from us in physical space (that is, how long their path is), so this method gives us a picture of the part of the network physically close to us (starting with our direct peers and branching outward).
2. Every 30 seconds, we perform maintenance actions in the janitor that deal with the xor keyspace stuff. We identify the furthest possible address from us (in terms of keyspace distance) to which we do not know of a valid next hop, and search for it. Read up on the Kademlia DHT if you want to know more (there's some implementation details in this document, but it's probably the most approachable beginners guide to kademlia that i've ever seen: https://github.com/telehash/telehash.org/blob/master/dht.md ). We don't do exactly the same thing (no k-buckets, slightly different implementation details) but the basic idea of an xor-based distance metric and identifying holes by organizing things bitwise is basically the same.

Finally, about the nodestore in the crashey branch. It now has a size that's small enough that we don't just keep the entire network in memory (it's 128 nodes by default now, and we have ~500-ish on hype last time I heard). So if the nodestore is full and we discover someone new, we evict the worst node. The details of how "worst" is defined are kind of complicated. It first looks at the set of nodes that are unreachable, and selects the one furthest from us in keyspace (by the xor metric). If no nodes are unreachable, it looks at the nodes that have no children (that's a nodestore thing, basically... nodes that aren't an intermediate hop on the best path to any other known nodes) and it evicts whichever one is furthest in keyspace.

Somewhat related... in theory, people in the same meshlocal could deliberately brute force addresses where the most significant bits in keyspace are a match (that is, that are close together). If somene outside the meshlocal doesn't know a route their destination inside it, but does happen to know of another node somewhere in the meshlocal, then they'd generally forward it to the right meshlocal at least. But I wouldn't advise actually doing this--it *shouldn't* be needed, so doing it now could actually hide any bugs in the current DHT logic and make things worse in the long run. (Also, for reference, addresses are rotated 64 bits before being xored. I have no idea why cjd decided to do this, as it makes distance calculations kind of annoying, but there you go. If ever we make a backwards incompatible change, I'm going to lobby that we take the opportunity to drop the 64 bit rotation. If the problem is the leading fc, then we should just do a bitwise reversal of the whole thing, to put the fc at the last position where it will never show up in a... ~thousand years of use)

Ansuz: lol, oh so every address would end in cf? or that's a hash.. so maybe not?

Arceliar: Addresses would still start with fc. right now, before we xor, we basically swap positions of the first and second half of the address, to keep the fc away from the first few bits because the DHT would hate that. My point is, if that's what we're worried about, we could keep the same addresses but flip them (so it would look like it ends in... not cf but whatever the bitwise reverse of fc is) (its.. 03) right before we xor. Nobody outside the cjdns code base would have any idea that this happens. It would just be slightly less confusing to work around than a 64 bit rotation... that or we chop the leading fc off of things before calculating anything. Implementation details, don't worry about it.

Ansuz: I was going to say we could make a script based off of:

~~http://ansuz.syrinxist.org/share/ArceliarAMA/vanitygen.sh.txt~~

https://wiki.projectmeshnet.org/Tips_and_Tricks#Picking_a_memorable_IPv6_address 

but if it's gonna mask bugs I won't bother

## Best number of peers to have?

So far I wrote this: 

~~http://ansuz.syrinxist.org/faq#architecture~~

http://ansuz.transitiontech.ca/faq#how-many-peers-should-i-have- 

but I'd love to have some facts to back it up

"if you're on a laptop that you travel with, you probably don't want to  route for people. For instance, if you occasionally tether your phone,  you might accidentally run up your data bill if you aren't mindful of  the traffic you are routing. In such a case, keep only one peer, and you  will never be a path, only a leaf node. "

Arceliar: That's not entirely true... your switch won't have to forward packets, but your router might. For example, if I'm trying to DoS reptoidz, but don't know a route to his IP, I'll forward the packets to whoever is the closest node I do know. If that's your tethered laptop, then you'll be asked to forward the packets. (This is how kademlia, and DHTs in general, usually work). One of the things on the to-do list is to come up with a *good* way to do a router lookup instead of blindly forwarding. So instead of forwarding the packet to you, I'd ask you if you know a route to reptoidz. I'd use this to produce a switch route that goes through you, but I'd notice the loop and splice it out. So you wouldn't end up forwarding any traffic, just answering DHT lookup requests. (Actually, that lookup would probably happen in parallel, in the background, while I blindly throw packets in your direction to be forwarded. I'd just stop asking you to forward them if/when I get a useful response from the DHT lookup. Note that crashey briefly *was* doing this, but it was using a CPU-and-bandwidth expensive search request. I can think of better ways to do this, but the "right" one would require addressing other problems first...)

The rest of the peering advice looks in general good. My only real comment is that leaf nodes (above) can't do useful routing at the switch level, and forwarding traffic at the router level is slower so we'd like to avoid it when possible. Having 2 peers lets you do things at the switch level but it's not especially useful (those guys could just directly peer instead, unless they're physically out of range in a meshlocal of course, in which case you're doing good). So 3 is the minimum number of peers before your switch actually gets to make non-trivial decisions about where to route packets. So if you're someone who wants to help the network, I'd say 3 peers (preferably ones that are not already peered with eachother) is the minimum I would shoot for.

<3 peer review

## Also:

Randati: I want to start doing analytics on the network based on
### what is the optimal ratio of nearby nodes to far off nodes? http://en.wikipedia.org/wiki/Small-world_network
depending on what type of node you have (mobile/home/VPS)
Having some kind of API to pull information of the network might be useful.

also: I've been playing with sigmajs, but d3js.org looks like it has some really useful stuff
http://christophermanning.org/projects/building-cubic-hamiltonian-graphs-from-lcf-notation/

## Are there any other types of nodes I haven't addressed?

```IRC
damn, u the best Arceliar <3

I'm reading through the whitepaper now,

<ansuz> ping for fixing routing: probably not magic
<ansuz> probably not placebo
<ansuz> Arceliar, what's your opinion on the matter ^^
* Arceliar reads scrollback
<ansuz> Alice can't find bob by browsing, Bob blasts pings at Alice, helps Alice find Bob
<ansuz> -> starts working
<Arceliar> ah
<ansuz> as in, generic network traffic to encourage peer discovery
<Arceliar> yeah. when alice receives a ping, she sees it's to a node that she doesn't have on her routing table yet. so she adds that node and the return route for the packet to the routing table.
<Arceliar> i think something approximating that happens. may have changed during crashey.
<ansuz> http://couch.syrinxist.org/share/fuck-yeah-l.png
<Arceliar> right now the network depends a lot on forwarding. if neither alice nor bob have the other on their routing tables, then never find eachother (unless it's by accident) and just depend on the DHT to handle routing
<Arceliar> if the DHT has problems (e.g. all the old nodes that haven't updated yet) then forwarded packets may vanish into a routing blackhole
<frozen> ansuz: that image is as large as it is to troll, isn't it.
<ansuz> lol
<ansuz> maaaaybe
<ansuz> umadbro?
...
<Arceliar> ideally, when we forward traffic, we'd keep the switch label from previous hops. so Alice->Eve->Bob would arrive at bob with Alice<-Eve and Eve<-Bob return labels attached, and Bob could splice a full route to Alice.
<Arceliar> but to do that, we first need to be able to stack switch labels (which we need to do anyway because 64 bits won't be enough if there's 10 billion nodes in the network)

<Arceliar> if Bob knows the Bob->Eve and Eve->Alice routes, he can splice Bob->Alice. but he's currently too stupid to ask for Eve->Alice.
<Arceliar> and Eve is too stupid to include it in case he'd find it useful
<Arceliar> at one point I had it set up so if Alice didn't know a route to Bob but tried to send a packet to him, she'd also trigger a search for him. which would get her a route. but it killed performance.
<Arceliar> i mean... Alice would find a great route, but tons of spammed searches everywhere, so it wasn't worth the cost
```
