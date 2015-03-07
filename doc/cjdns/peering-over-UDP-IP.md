# Peering over UDP/IP

[Outbound Peer](#outbound) --> UDP/IP --> [Inbound Peer](#inbound)
(The arrows only refer to the _initiation_ of the connection. Information flows both directions)

Structure:

- [Dynamic IP](#dynamic-ip)
- [cexec](#cexec)
- [Adding Peers](#add-peers)
    - [Information gathering](#info-gathering)
    - [Inbound Peering](#inbound)
        - [Authorized passwords](#authorizedpasswords)
        - [connectTo block creation](#connectTo)
        - [Edit cjdroute.conf](#edit-cjdroute-inbound)
        - [Update cjdroute](#update-cjdroute-inbound)
        - [Update your peer](#update-peer)
    - [Outbound Peering](#outbound)
        - [Edit cjdroute.conf](#edit-cjdroute-outbound)
        - [Update cjdroute](#update-cjdroute-outbound)

Prerequisites:

- cjdroute
- a network to peer over
- _preferably_ a static IP
- some time

If you have a static IP, reading this document in-order should make learning about what needs to be done easy and logical. If you experience disorentation or the urge to start drinking, stop and please drop by the IRC

## Dynamic IP <a name="dynamic-ip"></a>

Giving other people your IP address will not be a good way for your peer to reach you over the UDP/IP layer because, your IP address is subject to change without notice. However, if you know of someone with a static IP you can still connect by making an **outbound** connection to them.  
**tl;dr** don't choose an option below where you give out your IP address.

## cexec <a name="cexec"></a>

If you have any trouble in this section it is safe to skip but, you will need to use another tool or shutdown your cjdroute to add a new peer.  

Usage:

- make a file here `~/.cjdadmin`

put something like this inside:

```script
{
    "password": "from your admin section in your cjdroute.conf",
    "config": "/path/to/cjdroute.conf",
    "addr": "127.0.0.1",
    "port": 11234
}
```
make an alias:
```script
alias cjdadmin='/path/to/cjdns/contrib/python/cexec'
```
If you like you can put it in your `~/.*rc` for your terminal flavor.

## Adding peers <a name="add-peers"></a>

It is essential at this point in time that peers stay connected the network is small and subject to do strange things.  
Reasons you should have your peers contact information:

- Network diagnosis
- Network community
- You can contact them if you need to move your node

### Information Gathering <a name="info-gathering"></a>

- IP address can be found by a google of "whatismyip"
- public key is right here:

```script
    // This key corresponds to the public key and ipv6 address:
    "publicKey": "3u2fz3fcyblrz7nspwzkcxp7xph80h5hwu1qu4qrumrqym80r0u0.k",
    "ipv6": "fc5e:8c49:b6ce:d1f5:b89c:fcd6:e536:a479",
```

- port is right here:

```script
    "interfaces":
    {
        // The interface which connects over UDP/IP based VPN tunnel.
        "UDPInterface":
        [
            {
                // Bind to this port.
                "bind": "0.0.0.0:43653",

                // Nodes to connect to.
                "connectTo":

```


### InboundPeering<a name="inbound"></a>

You give your peer your IP address, port, and password.  
Prerequisites:

- A long passphrase that you do not have to remember
- Your pubkey
- Your static IP
- The open port your cjdroute is listening on
- Any other information you would like for your peer to have

#### Make your authorizedPasswords block <a name="authorizedpasswords"></a>

```script
        {
                "password": "vt1ly5f4ydmm9gjk196t160z23t6uju",
                "name": "[put your peers name here]",
                "contact": "[put your peers contact info here]"
        },
```
Things that are parsed:

- password

Everything else is for humans.

#### Make your peers connectTo block <a name="connectTo"></a>

(alredy indented just enough for the current cjdroute.conf section it goes in)

```script
                        "[your IP address]:[your open port]": {
                                "password": "[your long passphrase]",
                                "location": "New York City, NY, US",
                                "[insert cool noun here]": "[something witty]",
                                "contact": "[email address, phone number, ICQ, jabber take your pick]"
                                                "ipv6": "fcth:isis:notE:ssen:tial:butg:oodt:know",
                                                "publicKey": "thisIsFromYourCjdrouteConfAtTheTopItEndsWithADotanda.k",
                    },
```
Things that are parsed:

- IP address and port number combo
- password
- pubkey

Everything else is for humans.

#### Edit cjdroute.conf <a name="edit-cjdroute-inbound"></a>

After verifying that your made up password matches the password you will give to your friend, paste your authorizedPasswords block into your authorizedPasswords section.

Like this.
```script
   //
    "authorizedPasswords":
    [
        //note the comma added to the end of each block
        //default password
        {"password": "pnc8q05llp9sx7d1b4bc3d6ru0krgbl"},
        {
                "password": "vt1ly5f4ydmm9gjk196t160z23t6uju",
                "name": "[put your peers name here]",
                "contact": "[put your peers contact info here]"
        },

        // More passwords should look like this.
        // {"password": "tjrwwlsh4ugddk032yu8vrnv11v8z5f"},
        // {"password": "pm0643f911j71w0pctj5s7bkk0s8htv"},
        // {"password": "2vls52j3q4151dk8h2kz939kt0ldu75"},

        // Below is an example of your connection credentials
        // that you can give to other people so they can connect
        // to you using your default password (from above)
        // Adding a unique password for each user is advisable
        // so that leaks can be isolated.
        //
        // "your.external.ip.goes.here:43653":{"password":"vt1ly5f4ydmm9gjk196t160z23t6uju","publicKey":"3u2fz3fcyblrz7nspwzkcxp7xph80h5hwu1qu4qrumrqym80r0u0.k"}
    ],
```
Save cjdroute.conf

#### Update cjdroute <a name="update-cjdroute-inbound"></a>

There are 2 ways to do this.  
`sudo killall cjdroute`
Or
```script
cjdadmin 'AuthorizedPasswords_add("[YourLongPasswordYouWillGiveAway]","[peerUserName_aka_frendlyname]",1,0)'
```
Find out more about this command in the [admin/README.md][cjd-admin-readme] ([clearnet][cjd-admin-readme-clearnet]) section of the cjdns repository.

#### Update your peer <a name="update-peer"></a>

Securely transfer the connectTo block you created for your peer.  
Ways to do this:

- Get [GPG][]
- Get [OTR][]
- Get [Tox][]
- Write it down and walk to their house.

### Outbound Peering<a name="outbound"></a>

You will receve a connectTo block through secure means.  
Prerequisites:

- A peer that has agreed to accept an inbound connection
- A connectTo block from your peer

#### Edit cjdroute.conf <a name="edit-cjdroute-outbound"></a>

Paste your peers info inside your connectTo block.
```script
                // Bind to this port.
                "bind": "104.131.101.214:61522",

                // Nodes to connect to.
                "connectTo":
                {
                        // Note the comma at the end of their block
                        "[address]:[port]": {
                                "password": "[longPasswordYourPeerGaveYou]",
                                "location": "New York City, NY, US",
                                "hostname": "example.com",
                                "contact": "Alice@gmail.com"
                                                "publicKey": "ThisisFromTheirCjdrouteConfAtTheTopItEndsWithADotand.k",
                                                "ipv6": "fc00:0000:0000:0000:0000:0000:0000:0000",
                    },
                    // Add connection credentials here to join the network
                    // Ask somebody who is already connected.
                }
```
Save cjdroute.conf

#### Update cjdroute <a name="update-cjdroute-outbound"></a>

There are 2 ways to do this.  
`sudo killall cjdroute`
Or
```script
cjdadmin 'UDPInterface_beginConnection("[yourPeersPubkeyGoesHereItEndsWith.k]","[address]:[port]",0,"[longPasswordYourPeerGaveYou]")'

```
Find out more about this command in the [admin/README.md][cjd-admin-readme] ([clearnet][cjd-admin-readme-clearnet]) section of the cjdns repository.

## questions?



[Tox]: https://tox.im/
[OTR]: https://otr.cypherpunks.ca/
[GPG]: https://www.gnupg.org/
[cjd-admin-readme]: http://gitboria.com/cjd/cjdns/blob/master/admin/README.md
[cjd-admin-readme-clearnet]: https://github.com/cjdelisle/cjdns/blob/master/admin/README.md
