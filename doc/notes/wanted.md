# Help Wanted

First and foremost, we need someone to constantly review this repository, and keep this file up to date.

If you see something listed here that has been taken care of, please knock it off the list. If you see something here, and take care of it yourself, please do us another favour and cross it out and link to your work.

* cjdns
  + breakdown of 3 parts of cjdns as explained in the [whitepaper](https://github.com/cjdelisle/cjdns/blob/master/doc/Whitepaper.md)
  + how to [cjdroute.conf](https://wiki.projectmeshnet.org/Cjdroute.conf)
  + Admin API
* Community
  + latest news
  + current public peers policy (with links to peers)
  + public peers vs open peers (dedicated passwords)
  + abuse policy
  + what is _official_?
* Peering
  + generating and transferring credentials
  + ~~over UDP/IP~~
  + over Ethernet
  + over Wireless
    - [batman-adv](http://www.open-mesh.org/projects/open-mesh/wiki)
    - [oslr/commotion](https://en.wikipedia.org/wiki/Optimized_Link_State_Routing_Protocol)
    - [802.11s](http://lars.meshnet.berlin/view/welcome-visitors/view/80211s-instead-of-adhoc)
    - [Adhoc](https://wiki.projectmeshnet.org/Wireless_Autopeering_Tutorial)
  + over TCP/IP
    - allows obfsproxy deployment
  + in datacenters (private networking)
  + over iodine
* Security mechanisms (seccomp, angel/core, etc.)
* WiFi Recommendations
* cjdns/contrib/ scripts
  + dumpTable
  + systemd/upstart
  + peerStats
  + python/cjdnsadminmaker
* the build system :(
  + cjdns/node_build/make.js
* in the media
* [FAQ](../faq/)
  + how does cjdns choose which path to take? latency? number of hops?
    * choosing paths presupposes knowing them, which can get in the way of finding the _optimal_ path. Paths to nodes are stored preferentially based on the [xor metric](../cjdns/functions/Address_xorcmp.md).
    * the path choice metric is a combination of various factors (latency and number of hops are among them). This is poorly documented, and we need someone to explore the exact details. Contributions should go in `../cjdns/functions/`
  + My service doesn't like ipv6. how can I get it to run on hype? [6tunnel?](http://toxygen.net/6tunnel/)
    * inet6 with tcp6 and netcat
  + How can I help?
    * documentation!
    * coding
      - C - help build cjdroute!
      - [Android](https://github.com/berlinmeshnet/cjdns-android)
      - Go - help jph build zlarkd!
      - Windows/CrossPlatform devs - Help us make cjdns portable to *all* the devices!
  + Jargon file
* [Known bugs && their circumstances](../bugs/index.md)
  + Failure to reestablish connection after disconnect/ipv4 change
  + [OS/distro-specific quirks](../bugs/distro-quirks.md)
    - OSX doesn't autopeer
