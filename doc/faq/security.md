## Security

```
16:39 < Erkan> well, I am not so sure about using VPS, tbh
16:39 < Erkan> your hoster (or the agency showing him the judge's paper) can get your config
```

Suppose you want to [pull a snowden](http://en.wikipedia.org/wiki/Global_surveillance_disclosures_%282013%E2%80%93present%29) and release some sensitive documents. Suppose that for some reason you're relying on an experimental protocol (cjdns) to do so. `Erkan` is wondering about the wisdom of doing so.

[cjdns](https://github.com/cjdelisle/cjdns) is big and complicated, and without a fair deal of experience with cryptographic tools you may have a hard time understanding what's safe and what is not.

## If you're hosting a cjdns node on a VPS

There are quite likely measures that you can take to encrypt your VPS's hard drive, and you should look into that, but suppose you haven't encrypted anything. In that case, yes, your VPS provider can probably just go and look at your private keys, and read your encrypted traffic.

What you should understand is cjdns uses [end to end encryption](http://en.wikipedia.org/wiki/End-to-end_encryption). That means that having your own private key compromised only affects that particular node's security. A compromised node can continue to forward traffic, and because of the nature of public key cryptography, it will be unable to read the contents of the packets it relays.

So, should you worry about hosting on a VPS? **Only if you intend to _send_ or _receive_ sensitive information on that particular node**.

If you want to connect to Hyperboria, but are for whatever reason unable to do so from your main computer, using a VPS as a static link into the network is quite reasonable. As long as you take precautions to protect the private keys on any nodes which initiate connections, **your data will remain secure in transit**.

## A few notes

cjdns is experimental! It is possible that someone is using a zero-day exploit in the wild to compromise a node's security. 

If you're really worried, you can also use tools like PGP to further encrypt data before sending it.

You could also use an [XMPP client](http://en.wikipedia.org/wiki/XMPP) that supports the [OTR](https://otr.cypherpunks.ca/) protocol, and use that to transfer PGP-encrypted data over cjdns.

If you have good reason to protect your data (other than simply doing it on principal), then redundancy is your friend!
