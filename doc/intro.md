# What the fc00::/8
## a brief introduction to cjdns

author: [kpcyrd](http://github.com/kpcyrd)

original location: https://github/ansuz/cjdns-pr

```quote
We have the means and we have the technology to end mass surveillance without any legislative action at all, without any policy changes. By basically adopting changes like making encryption a universal standard - where all communications are encrypted by default - we can end mass surveillance not just in the United States but around the world.
- Edward Snowden
```

## The current internet

 - Addresses get spoofed
 - Services get censored
 - Networks get hijacked
 - Controlled by evil corps
 - **Everybody can read yo packets**

## Where we're going

- Encrypts all your packets
- Almost zeroconf
- No authorities like IANA
- Clean IPv6 networking interface
- Enough addresses for everybody

## Sounds complicated

1. Install cjdns
2. Find a friend to connect to

## Friend to Friend

- Nobody knows who's behind an address since there's no registry
- Your friends know, though
- In case of abuse your friends may stop peering with you

## Peering (The simple way)

1. Get a friend who's connected
2. Get a cable
3. Connect the cable to each computer
4. Enable autopeering
5. Connection established

## Peering (Over the internet)

1. Configure an IP address on both sides
2. Ensure one is able to connect to the other
3. Exchange peering credentials over a secure channel
4. Encrypted traffic will be encapsulated in UDP packets

## How is it secure?

There's no central trust authority.

**fc42:18f1:763b:9b70:0059:9896:ef20:6de7**

All those random letters are used to verify your identity.

The address is the cryptographical fingerprint.

## ICANN hax it

- Don't use DNS to distribute your IPv6
- DNS can be hijacked
- Thus, cjdns addresses can get spoofed

If we use the current domain name system it's not very secure

## Namecoin to the rescue

- Fully decentralized
- Stores all addresses in an uncensorable blockchain
- Cryptographically ensures authenticity

## Next steps

- Android devs - Help going mobile
- Peers - Help the networks health
- Services - Help providing more useful stuff
- Windows devs - Help the lost souls

All skill levels wanted
