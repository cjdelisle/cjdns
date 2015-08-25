## What is Hyperboria?

[Hyperboria](http://hyperboria.net) is an encrypted [Mesh Network](http://en.wikipedia.org/wiki/Mesh_networking) designed for privacy and resiliency to censorship.

It currently exists as an [Overlay](http://en.wikipedia.org/wiki/Overlay_network) test network for [Project Meshnet](https://projectmeshnet.org/), and is only accessible to those who install [cjdns](http://en.wikipedia.org/wiki/Cjdns).

Ultimately, we hope to build a viable alternative to the regular internet, which we call [clearnet](http://www.urbandictionary.com/define.php?term=clearnet). Our ultimate goal is to replace the existing hierarchical internet with a non-hierarchical model.

You are encouraged to set up your own means of communication that does not rely on the internet, maybe using something like [this woktenna](http://geobray.com/2010/02/07/woktenna-for-3g/)?

In order for this to be plausible, we require a sufficiently dense number of nodes. Neighbouring nodes need to be able to communicate with each other somehow.

Chains of nodes are vulnerable to being shut off if even one link is taken offline, so we aim for clusters of interconnected nodes.

This is called a [Meshlocal](/locals). I am trying to start a [Toronto-based MeshLocal](/toronto).

To join the network, you need a password and a public key from someone who is already on the network. If you're in Toronto or the surrounding area, and are interested, <a href="/contact">contact</a> me.

## Can I run Cjdns in a virtual machine?

You can, but you might have trouble finding willing peers, since you're not really contributing much to the network by having a 'sometimes-on' node.

## Why is it taking so long to find a peer?

This project is run entirely by volunteers. In fact, 'run' might give you the idea that it's some kind of beureaucratic body. It isn't. We argue about things, sometimes <a class="clearnet" href="http://www.bestbytepc.com/uploads/2/6/6/1/2661857/7237729_orig.jpg">IN ALL CAPS</a>. Sometimes people <a href="https://encyclopediadramatica.es/Quitting_IRC_forever">QUIT IRC FOREVER</a>. We reach a consensus, or we don't, and people just do what they want. Actually, even if we do reach a consensus, we're still just doing what we want. If you expected something other than anarchy, I encourage you to read more about what a meshnet is.

That means you should be patient, since you're relying on other people's good will (crazy, I know). This is a friend of a friend network, which shocks people, because they actually have to make friends. Tell us about yourself. Where are you located? Do you operate any servers, or are you new to this? If you're new, why do you want to learn? Maybe you maintain a <a class="clearnet" href="http://blag.xkcd.com/">blag</a> or something? That's pretty much what we do on Hyperboria. If you aren't interested in doing that kind of thing, but want access, I'm not sure what your motivation is. We're basically a bunch of DIYers making our own internet. That kind of thing takes time. Think in terms of years, not months. Read <a class="clearnet" href="http://en.wikipedia.org/wiki/Internetwork">this</a> and <a class="clearnet" href="http://en.wikipedia.org/wiki/History_of_the_Internet#Three_terminals_and_an_ARPA">this</a> to get a sense of context for what this project is about.

With that in mind, say hi, tell us what you're doing. We're pretty enthusiastic about sharing our interests if it's unlikely that you're a <a class="clearnet" href="http://www.urbandictionary.com/define.php?term=feds">FED</a>.

Read [this document](/cjdns/peers) on peering for more info.

## Why should I prefer the friend of a friend model?

Hyperboria is a pseudonymous network, your direct peers can tell where you are (by virtue of their connection), no one else can (unless you expose your data personally).

A network that presupposes this principle is resistant to censorship because people cannot simply find you and punish you for your opinions. Many of the links that make up our network cross between legal jurisdictions, and are not subject to the whims of a single government.

Other networks which do more to guarantee absolute anonymity tend to attract behaviour which would be reprehensible in almost any other social situation. We are proud of our network's ability to promote free speech, while minimizing behaviour which violates human rights. Some have made the point that restriction of any kind of content is censorship, and our views are self contradictory.

The premise is simple: you get to decide who you connect to, provided they consent. It is not some centralized body which will decide to disconnect you if you use our bandwidth for malicious purposes. You are only accountable to your peers. The rest tends to sort itself out.

## Can I run cjdns from home (dynamic IP/NAT'ed connection)?

We describe it in terms of 'inbound' and 'outbound' peers

Inbound means people can connect to you, which requires either not being behind a router, or being able to forward ports

Anyone who can install and run cjdns can be an outbound peer

## Do I need to exchange credentials with all my peers?

No, you do not. One of you needs to have an accessible IP in order to establish a connection, but once connected, traffic flow is bidirectional.

It won't hurt for you to add each other to your conf file, but unless one of your IPs suddenly changes, there isn't much benefit to the redundancy.

## What information should I provide with my peering credentials?

<a href="https://wiki.projectmeshnet.org/User:Thefinn93">thefinn93</a> recommends offering <a class="clearnet" href="http://jsonlint.com/">valid JSON</a>, including fields labelled:

```JSON
"your.ip.add.ress:port":{
  "publicKey":""
  ,"password":""
  ,"IPV6":""
  ,"location":""
  ,"contact":""
}
```

Technically, only the IP, port, publicKey, and password are required. IPV6 will make it easier to keep track of who is connected, and what kind of latency they are experiencing. The location helps your peer direct possible local meshers your way. Contact should be some non-hyperborian method, in case your connection breaks. <a class="clearnet" href="http://en.wikipedia.org/wiki/Simple_Mail_Transfer_Protocol">Email</a>, <a class="clearnet" href="http://rows.io/">XMPP</a>, <a href="/gpg">PGP</a>, <a href="https://keybase.io/">Keybase.io</a>, <a href="/contact">etc</a>.

## Beyond my first peer, how can I set up a more efficient network?

[Read this](http://en.wikipedia.org/wiki/Small-world_network)

People are much more receptive to peering requests if you have already connected to Hyperboria.  EFNet has a much wider audience, and as such is treated with skepticism. Those with their foot already in the door will have an easier time making friends. If you think this is clique-ish or elitist, you may be right. If you want it to change, then <a class="clearnet" href="http://www.elephantjournal.com/2011/08/be-the-change-you-wish-to-see-in-the-world-not-gandhi/">be the change you want to see in the world</a>, and get on EFNet and start helping!

It's  a bit circular, but it's a not a rule we have, more of an emergent behaviour. Many of us have spent a lot of time on EFNet helping newcomers to connect, only to have them leave after connecting for fifteen minutes.  Once we've seen that you are competent enough to connect, and that you have some lasting interest, there is far more incentive to spend time getting to know you.

As to how the technical specifications of the network architecture...

## How many peers should I have?

The number of peers you should have depends on a number of factors

if you're on a laptop that you travel with, you probably don't want to route for people. For instance, if you occasionally tether your phone, you might accidentally run up your data bill if you aren't mindful of the traffic you are routing. In such a case, keep only one peer, and you will never be a path, only a leaf node.

If, however, you are trying to peer with, say, a home connection with a decent data cap and throughput, you may want to add a few more connections. These ought to be limited to fairly local peers, as latency can add up fairly quickly with each successive hop. Home connections are primarily where the wireless aspect of the meshnet comes into play, since neither your laptop or VPS will likely be able to connect to a mid-range wireless antenna. If you have a clear line of sight to your peer (you live on a hill, or have really cool neighbours), all data travelling over this connection is free! Make use of that! Put each other on IP whitelists with access to higher bandwidth services. A meshlocal is capable of a lot of things that become difficult at the scale of a global meshnet.

If you're peering up a VPS with high throughput and unlimited data, you have the capacity to do things most residential connections can not. You are capable of acting as a highway for the meshnet. Keep in mind that it can be a bad idea for highway traffic to have direct exits into residential areas (metaphorically speaking). It's a good idea to use your connection to establish long range links between your meshLocal and others in far off places. Trans-Oceanic links help link together a greater number of node operators across the globe, and allow us all to communicate what kinds of issues we are each experiencing.

It would be unrealistic to expect any network to be completely uniform. The optimal configuration changes over time according to the habits of its many users. Healthy configurations look less like a <a class="clearnet" href="http://en.wikipedia.org/wiki/Spoke-hub_distribution_paradigm">spoke-hub distribution</a> and more alike a river, or tree. It is possible to have an efficient hierarchy without being centralized, just avoid <a class="clearnet" href="http://en.wikipedia.org/wiki/Single_point_of_failure">SPOF</a>s.

This may sound a lot like urban planning to you. It is! Consider <a class="clearnet" href="http://en.wikipedia.org/wiki/The_100-Mile_Diet">The 100-Mile Diet</a>. You don't want to have to 'drive halfway across town to get to the grocery store'. We are simply trying to mitigate the unpleasant side effects of urban sprawl. We are planning for <a class="clearnet" href="http://en.wikipedia.org/wiki/Walkability">walkable neigbourhoods</a>.

If there is only one grocery store in a 400-mile radius, then everyone needs a car. Prime real estate (that which is close to the store) becomes quite expensive, while it is cheaper, yet <a class="clearnet" href="http://en.wikipedia.org/wiki/End-to-end_principle">inefficient</a>, to live on the fringe. Following this metaphor, ISPs are basically a grocery delivery service. If they decide not to deliver your groceries (remember <a class="clearnet" href="http://beattheblockade.org/">the wikileaks blockade</a>?) you starve. We want to help you plant a community garden, where everyone can benefit - as long as they contribute their fair share. I may have no reason to just give away vegetables that I worked hard for, but I'd be happy to trade for a different crop simply because I enjoy variety.

## How can I generate passwords for new peers?

Easy, just run the following command:

<code>cat /dev/urandom | strings | head -n 20 | tr -d '\n"`\\ \t'  | head -c 40 && echo</code>

## Can I use UPNP to bypass NAT?

cjdns does not interact at all with your NAT setup. Use some other upnp client to control your router.

## Can I substitute a domain for an IP in my connectTo?

cjd originally had no intention of implementing this, but someone<a href="/contrib">(?)</a> implemented it and submitted a pull request. Now you can.

## Is there a meshlocal in my vicinity?

Check <a href="/locals">this page</a> for a basic list of some general areas. It is not an exhaustive list. If you'd like to get listed, check <a href="/contrib">my contributions page</a> for an idea on how to do that.

## Can someone give me peering info?

```text
01:53 -!- newguy [webchat@198.20.69.234] has joined #cjdns
01:54 < newguy> can someone point me toward a public peer?
01:55 -!- newguy [webchat@198.20.69.234] has quit [Client Quit]
```

We see this **a lot**. You should read [ircerr's peers.txt](/peers), [the peering section of my old faq](/faq#peering), and [my page about meshlocals](/locals).

TL;DR :: this isn't a public network, and we're under no obligation to give credentials to everyone who asks. We aren't a non-profit, or a corporation, or even a cohesive group. Everyone here has their own motivations and ideas of what the network should be, and you need to find someone who wants to connect to you. Don't be shy, introduce yourself.

## My peer's connection details aren't working, what's wrong?

I don't know, maybe start by asking your peer? If you don't know how to contact your peer, then you may have just learned a valuable lesson. Take down your next peer's info so that you can tell them when something is wrong. If somebody gave you credentials to connect, then you are officially **their problem**.

## How can a figure out which peers I am directly connected to?

```JSON
{
    "addr": "127.0.0.1"
    ,"port": 11234
    ,"password": "notAnActualPass.UseYourOwnFoundInYourConf"
}
```

Make a file called `~/.cjdnsadmin`, containing valid JSON with the properties above. These credentials will be used by any scripts which need to connect to the admin interface to gather data.

To find your peers, run `cjdns/contrib/nodejs/tools/peerStats.js`

Alternatively, you can use [this tool which does a few other things as well](https://github.com/ehmry/cjdcmd-ng).

## How to sniff for ethernet cjdns traffic?

```
tcpdump -nn -s0 -t -vv -e -i mon0 ether proto 0xfc00
```

## What are the EAGAIN errors about?

This is a red herring. It simply means that UDPInterface received a packet, and that there's not immediately another packet received.

Example:

```
recvmsg(13, {msg_name(16)={sa_family=AF_INET, sin_port=htons(25021), sin_addr=inet_addr("31.20.45.26")}, msg_iov(1)=[{"\0\0\0\1\1\312\250\207\206\6p\230\200\0k\333\305(\204\27`0\370\202\332B\341`89\376\210"..., 3496}], msg_controllen=0, msg_flags=0}, 0) = 160
recvmsg(13, 0x7fff9490f030, 0)          = -1 EAGAIN (Resource temporarily unavailable)
```
