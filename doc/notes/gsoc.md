## cjdns google summer of code

[Google Summer of Code](http://www.google-melange.com/gsoc/document/show/gsoc_program/google/gsoc2015/help_page) has several goals:

1. Create and release open source code for the benefit of all
2. Inspire young developers to begin participating in open source development
3. Help open source projects identify and bring in new developers and committers
4. Provide students the opportunity to do work related to their academic pursuits (think "flip bits, not burgers")
5. Give students more exposure to real-world software development scenarios (e.g., distributed development, software licensing questions, mailing-list etiquette)

## Our goals

1. Get funding! Fixing cjdns is hard work, and it takes a lot of time. A precedent of funding from google might make it easier to pick up funding from other sources.
2. Get attention! People pay attention to projects that google funds. We need more eyes.
3. Structure! Hard deadlines get people moving. We'll get more done as a well-defined organization.
4. A working product... an android app? Something to improve the core routing algorithm? Debugging/visualization/simulation tools? Userland config management?

## Ideas

### cjdns on the Freifunk backbone (BBB) and network (IC-VPN)

The Berlin BackBone (BBB) is a network of dedicated wifi links between several sites at public buildings like churches or townhalls. It is the backbone of Freifunk Berlin's network, which other nodes in the neighbourhoods connect to. The InterCity VPN (IC-VPN) connects the Berlin network with the networks of other Freifunk communities, e.g. in Leipzig and Hamburg.

The goal is to:

- tunnel Internet traffic from the neighbourhood to the Freifunk <-> Internet exchanges
- link cjdns islands in the community
- automate the tunnel setup on leaf nodes

TODO: need to think about how remote-friendly this idea is

### Continuous simulations and benchmarks of cjdns

- [HACKING.md#simulating](https://github.com/cjdelisle/cjdns/blob/master/HACKING.md#simulating)
- [cjdns-testbed](https://github.com/lgierth/cjdns-testbed)

### DNS

See [DNS, with trust based on peering-like agreements](dns.html#dns-with-trust-based-on-peering-like-agreements)
