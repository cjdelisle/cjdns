# What does everything in your configuration file do?

If you've never worked with [JSON](http://en.wikipedia.org/wiki/JSON) before, you might feel a little overwhelmed editing your  **cjdroute.conf** for the first time.

One thing that makes matters more difficult is that your cjdroute.conf is **not** actually valid JSON.

This document will:

1. Indicate the exact purpose of each attribute of your configuration file.
2. Document the permitted and recommended syntax you can use, and the difference between the two.
3. Indicate which attributes are required, and which are optional.
4. Refer to related documentation about specific attributes, and the surrounding recomended practices.

## Structure

* privateKey
* publicKey
* ipv6
* authorizedPasswords
  + password
  + ipv6
  + user
* admin
  + bind
  + password
* interfaces
  + UDPInterface
    + bind
    + connectTo
  + ETHInterface
    + bind
    + beacon
    + connectTo
* router
  + interface
    * type
  + ipTunnel
    * allowedConnections
      + publicKey
      + ip4Address
      + ip6Address
    * outgoingConnections
* resetAfterInactivitySeconds
* security
  + setuser
  + exemptAngel
* logging
  + logTo
* noBackground
* dns
  + keys
  + servers
  + minSignatures

## Functionality

### privateKey, publicKey, ipv6

As indicated in [this article on cjdns' cryptographic functions](cryptography.md), your ipv6 is derived from your publicKey, which is in turn derived from your privateKey.

**cjdroute --genconf** produces a sample configuration file which includes all three, but they are not all actually required to launch your router.

Instead, cjdns only uses your privateKey, then derives the rest. As such, the publicKey and ipv6 are only there for your benefit.

### authorizedPasswords

This is an array of objects. Each object can contain a number of keys that specify how that password is to be used:

1. "password" specificies a password that your node will accept as valid. It's recommended that each peer be given a unique password. Without a valid password, a peer will not be able to connect.
2. "ipv6" specifies which ipv6 can be used in conjunction with a particular password. Additional nodes attempting to connect using the same password and a different ipv6 will be rejected.
3. "user" is an optional label which must be unique among passwords in your configuration file. It has no effect on peering, otherwise, but is used to label connections when using the peerStats API call. This human readable format makes it easier to determine how each of your peers is behaving, and whether they need to update.
