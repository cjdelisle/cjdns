# Glossary

## Proposed rules

This is an incomplete list of terms used in direct context of cjdns.

- Please add terms convering cjdns itself, tools and best practices, peering,
  community, Hyperboria, similar routing protocols --
  e.g. NodeStore, peer stats, WTFM, buildbot, 802.11s
- Please don't add terms which are out of scope --
  e.g. BitTorrent or Peer To Peer
- Abbreviations are okay, e.g. P2P, DHT, OLSR

## List of Terms

### Hidden Peers

This was a peering bug which was fixed in cjdns v14. Please make sure none of
your peers is affected, and if there is one, tell them to update.

    tools/peerStats | sort

### NodeStore

The internal name of cjdns' routing table.

### P2P

Abbreviation of Peer To Peer.
