# Ideas for new docs

* cjdns
  + breakdown of 3 parts of cjdns as explained in the [whitepaper](https://github.com/cjdelisle/cjdns/blob/master/doc/Whitepaper.md)
  + how to [cjdroute.conf](https://wiki.projectmeshnet.org/Cjdroute.conf)
  + Admin API
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
* FAQ
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
* Known bugs && their circumstances
  + Failure to reestablish connection after disconnect/ipv4 change