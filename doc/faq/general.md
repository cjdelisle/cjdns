## What is Project Meshnet?

Project Meshnet is an organization that aims to build a versatile, decentralized network built on secure protocols for routing traffic over private mesh or public networks independent of a central supporting infrastructure.  
  
## What is Cjdns?

Cjdns (Caleb James DeLisle's Network Suite) is a networking protocol and reference implementation. It is founded on the ideology that networks should be easy to set up, protocols should scale smoothly, and security should be ubiquitous.  
  
## How do I install Cjdns?

Instructions are on [github](https://github.com/cjdelisle/cjdns)

## What is Hyperboria?

[Hyperboria](http://hyperboria.net) is an encrypted [Mesh Network](http://en.wikipedia.org/wiki/Mesh_networking) designed for privacy and resiliency to censorship.

It currently exists as an [Overlay](http://en.wikipedia.org/wiki/Overlay_network) test network for [Project Meshnet](https://projectmeshnet.org/) and is only accessible to those who install [cjdns](http://en.wikipedia.org/wiki/Cjdns). 

Ultimately we hope to build a viable alternative to the regular internet, which we call [clearnet](http://www.urbandictionary.com/define.php?term=clearnet). Our goal is to replace the existing hierarchical internet with a non-hierarchical model.

You are encouraged to set up your own means of communication that does not rely on the internet, maybe using something like [this woktenna](http://geobray.com/2010/02/07/woktenna-for-3g/)?

In order for this to be plausible, we require a sufficiently dense number of nodes. Neighbouring nodes need to be able to communicate with each other somehow.

Chains of nodes are vulnerable to being shut off if even one link is taken offline, so we aim for clusters of interconnected nodes.

This is called a [Meshlocal](/locals). I am trying to start a [Toronto-based MeshLocal](/toronto).

To join the network, you need a password and a public key from someone who is already on the network. If you're in Toronto or the surrounding area, and are interested, <a href="/contact">contact</a> me.  
  
## Is Cjdns anonymous?

No, Cjdns is not anonymous, nor is it intended to be. For a more in-depth explanation, take a look at the <a href="https://github.com/cjdelisle/cjdns/blob/master/doc/Whitepaper.md">whitepaper</a>.  
  
## I can ping||curl||wget a cjdns ipv6, but I can't browse to it. What's up?  

It's most likely a problem with your browser. If you are using Firefox||Iceweasel||Chrome||Chromium, we know about this, and there's a fix.  

In Firefox or Iceweasel, everything should work out of the box (on Debian and Ubuntu, at least). Mint is weird, and has for some reason disabled IPV6 by default in their Firefox package. You can turn it on by entering `about:config` in your omnibox. You may get some sort of warning about it being possible to break things, ignore it and proceed. Enter `IPv6` in the search field that appears. There should be an entry: `disableIPv6`, and it is likely set to `true`. Set it to be false, and just to be sure, close the browser completely and reopen it to test things out.

If you are using chrome or chromium, you probably need to relaunch the browser using a flag that enables ipv6. `chromium-browser --enable-ipv6` or `google-chrome --enable-ipv6` should do the trick.  

If you're having trouble browsing to a raw ipv6 string, keep in mind that different programs still use different formats for entering such ip addresses. Most browsers require that you enclose the address in square brackets, like `http://[fce3:4ece:ab11:505e:21aa:fc21:bf42:62b9]/` <- this.  

Some irc clients (Hexchat) need you to enter `fcbe:5f12:67d8:77ea:e4d8:aecc:2b4f:a4b/6667`  
  
## It's still not working  

It's also possible that everything's configured properly, but you don't have a route to the node you're trying to find. When this is the case, you can try to strengthen the connection according to <a class="clearnet" href="http://en.wikipedia.org/wiki/Hebbian_theory">Hebbian Theory</a>, which, in neuroscience, is the idea that 'cells that fire together, wire together'. You might not know a route to that node, but it might have one to you. Getting them to ping you, if possible, should give you a better connection. If they can't, have them ping nearby nodes that they can find. You should do the same. Consult <a href="http://www.fc00.org">www.fc00.org</a> to get a better idea of what nodes you should look for.  
  
## Is it possible for a central server to know routes to every other node?

There are limits to the size of any node's routing table, but at hyperboria's current size, one server can know it all. That won't remain true as it grows.  

## What is Hyperboria's solution for DNS?  

* HypeDNS
* RainflyDNS
* Parabolic DNS
* why not namecoin?

## What crypto does cjdns use?  

* Salsa20
* NaCl

## How can I host my own website on Hyperboria?  

* Apache
* Nginx
* NodeJS

## Can cjdns be made to automatically run||update?

* If you are using the master branch, things should be stable enough to periodically update. You probably shouldn't do this if you're using crashey. I prefer to run updates manually so I know when things have changed.
* There are many ways to set cjdns to always run in the background, though methods will vary depending on your system.

## Do I need to run cjdroute as root?

Short Answer: Pretty much

Longer Answer: You generally need root permissions to create the encrypted tunnel interface that cjdns uses to communicate with other nodes. Once that is created, all extraneous permissions are dropped. In any case, cjdroute is limited to reading/writing from 0 files, so even if someone could exploit it, they wouldn't be able to access anything of any value.

## What systems are supported?

Short Answer: not windows.

Long Answer: Check the dependency list. You need access to a C99 compiler, NodeJS, Python, LibUV...

## Can I use UPNP to bypass NAT?  

cjdns does not interact at all with your NAT setup. Use some other upnp client to control your router.  

## Is there a linux distribution that works best as a Hyperboria node?

Pretty much any distro will work, though some may require more tweaks.
