# nodeinfo.json

*A way for nodes to provide info about themselves and improve discoverability.*

This is an evolving standard, we've generally agreed on this format, but we'll
see what people do with it.

A JSON file at `/nodeinfo.json` that looks something like below. Parsers should
have no expecations about this file, in my tests found several 404 or other
non-JSON pages at that path returning HTTP 200. Further, all keys are optional.
Additional fields can me made up by other node operators. This spec
will change as we see how people use it. Please provide at least a contact
email, and ideally a PGP fingerprint with the full key on public keyservers.

Sample Files:
 * http://[fcef:96c2:7226:a4b1:97ea:59bf:cb2e:51f0]/nodeinfo.json
 * http://[fcec:ae97:8902:d810:6c92:ec67:efb2:3ec5]/nodeinfo.json


```json
{
  "key": "cjdnspublickey.k",
  "hostname": "your.nodes.fqdn",
  "contact": {
    "name": "Your name, handle, nick, whatever",
    "email": "you@domain.tld",
    "xmpp": "you@domain.tld",
    "other cool communication service": "whatever identifier they use",
  },
  "last_modified": "2014-11-30T00:35:48+00:00",
  "pgp": {
   "fingerprint": "FULL40CHARFINGERPRINT",
   "full": "http://link.to.download/full.key",
   "keyserver": "hkp://pgp.mit.edu"
  },
  "services": [
    {
      "uri": "http://[fc00:]/",
      "name": "My awesome http website",
      "description": "my blag and stuff"
    },
    {
      "uris": {
        "webInterface" :"http://example.tld/",
        "otherthing": "cactus://example.tld/"
      },
      "name": "MutliURI service",
      "description": "Some service with multiple URIs"
    }
  ]
}
```
