# Random notes about cjdns and DNS

by the incredible ansuz

THIS IS AN ETHERPAD Meaning you can just write stuff in if you want. Try not to delete stuff, feel free to add try to respect the overall structure, but it's backed up anyway if you go ruining shit, you aren't really doing any damage, just being immature. KTHXBAI

PS: IF YOU WANT TO GET CREDIT FOR ANY INFO YOU ADD, PUT YOUR NAME BESIDE IT. EVENTUALLY I WILL CLEAR AUTHORSHIP COLOURS FROM THIS DOC, AND ATTRIBUTION WILL BE IMPOSSIBRU

IMHO hype is in need of 3 things

- Docs  http://couch.syrinxist.org:9001/p/docs && http://couch.syrinxist.org:9001/p/howCjdnsWorks && http://couch.syrinxist.org/faq
- easily deployable email, since there are obvious problems with having it centralized, it needs to be easier for the user to host (specifically on hyperboria) I am hopeful about http://haraka.github.io/ and https://github.com/andris9/simplesmtp .. though again, a clear guide to setting up dovecot/postfix/whatever on hype would also be satisfactory. Eventually I plan to make something out of simplesmtp for myself (probably over the summer), and I'll be releasing it. Unless someone else does it first... HINT. oh and by easy I mean like run a script and log in. srsly.
- DNS.. see below

## DNS on hyperboria/parabolic/any cjdns network

### What is this about??

the curator of hyperboria's "hypeDNS" has gracefully retired from Hyperboria, leaving DNS in a state of transition.

Now might be a good time to actually figure out how we're going to move forward.

### Existing/competing services

(what do people think of pros/cons?)

- ICANN (by the NSA) # The DNS you already use, provided by clearnet
  - h.example.net for hyperboria, example.net for clearnet
  - doesn't work well with CNAME records, since they get used for AAAA resolution too
    - `pad.meshwith.me.  227  IN  CNAME  dax.meshwith.me.`
    - `dax.meshwith.me.  227  IN  AAAA  fcc6:700e:1577:b2df:20cc:3ece:24ff:3fa7`
    - `<larsg> should remove those CNAMES`
    - `<larsg> they apparantly get used for AAAA lookups too`
    - `<larsg> that's how pad. resolves to 3fa7`
    - `<larsg> finn: i mean CNAME records for domains which also have a h. variant, in general`
    - `<larsg> because due to the CNAME with an AAAA record on the other end, the clearnet variant is not clearnet anymore`
- hypeDNS (formerly by Mikey, now by finn) # conventional dns, centralized, only on hype
- paraDNS (by sdamashek/wolfmitchell) # central to the parabolic [sub]network, also conventional DNS, also only on hype
  - http://couch.syrinxist.org/dns
- rainflyDNS (ironically, by cjd) # built off of namecoin? integrated into cjdns. development (seemingly) halted # correct me if I'm wrong.
  - https://pad.meshwith.me/p/rainflydns #thx
  - http://gitboria.com/cjd/rainflydns/
  - http://rfdns.cjdns.ca/
  - https://ezcrypt.it/ck5n#gYdfOeREHwzD92H0WDry4Z5e
  - https://ezcrypt.it/9Q6n#ejSF9d762REt7hmJ8pl46yp7
- DJDNS   (ironically, by rainfly) # maybe a good idea? idk. not widely adopted, written in Go, so I'm not fond of it.# just trolling.. kinda..
  - https://github.com/campadrenalin/djdns
  - There's also https://github.com/campadrenalin/python-djdns
- raw ipv6 webring? (by anyone who wants to get involved) # just post links to things that people ought to be able to find. doesn't really scale well, basically just word of mouth.
  - advertised services using DHT? Hypertary? (by Igel? Frozen? idk) # My understanding of this is shallow at best
- GNUNet Name System
  - https://gnunet.org/taxonomy/term/34
  - https://www.youtube.com/watch?v=uhIybhAQ8lY (30c3 lecture)
  - https://gnunet.org/sites/default/files/schanzen2012msc.pdf (masters thesis proposing GADS) (read this if you need to kill an afternoon)
- .p2p (from TPB people) - http://p2pfoundation.net/Dot-P2P http://dot-p2p.org
- http://couch.syrinxist.org/share/dns-idea.txt #ircerr's idea

### proposed/extant TLDs...

- .h # nice and short
  - confusing when we consider that cjdns is heavily related to C
- .hype # very descriptive, has some history, but isn't widely adopted
- .para # has been adopted by 100% of parabolic, which isn't many people, but that's something, seems to be run with pride.
- .mesh # also descriptive, makes for nice vanity urls... seattle.mesh, toronto.mesh, etc.
  - is potentially bad because we are not the only mesh network # fuck da police, also, if it's internal to hyperboria it shouldn't affect anyone (assuming the links posted are also internal) # if other meshes are using ICANN, they are ostensibly doing it wrong. but then.. so are we.. soooo yea
  - .net # because why not go full troll mode (this is not a serious suggestion) <3
- ????  # idk, propose something yourself
- should/could be descriptive of what the site is for, such as .mail, .forum, .blog, .wiki  // there's the issue of 'poisoning' the namespace. eventually when clearnet increases the number of TLDs hype users might not be able to access clearnet sites. If they have to migrate a hype domain they've had for some time to something new because of an ICANN conflict, they'll be pissed. Hardlinkes are hard to get rid of. I'm in favour of having fewer hype TLDs (until we need more) to avoid this complication.


## What is our future dns solution going to look like?

seriously, if we keep waiting for something perfect, we will have too many competing services to get people to adopt 'the perfect option'

I'd like to start with anything now, get it adopted, then improve it. Premature optimization kills lots of great ideas. DNS is very important, and I want it to work.. as in, NOW.

### missing:

Why exactly DNS is important (people don't really care about it even on clearnet, see google).

- we don't have a search engine, and it's not possible to crawl blindly // we have http://[fcd6:e9a6:a7be:b875:edd7:dc39:abac:a1c7]/ <- Oscar, and jph might be working on something based off of elastic search, also, derp might have something on the way Also you can run YaCy and crawl things locally, and we could probably set it up to share over a hype-only YaCy DHT (that's how it works), but it's kind of a bitch to get it to crawl only IPv6/Hype addresses because of how it parses regex (badly) # Last I heard YaCy wasn't working on IPV6...  NVM http://ipv6-yacy.blogspot.ca/ .. even mentions hyperboria, is this from Vadipp?

Why a number of competing services ("healthy ecosystem"?) is bad. // bad like freedom, not beer

- user has to run too many servies and configure too many things; it's hard to see what's going on

These qualities listed below would all be nice things to have however, some of them might not be compatible with others. we need to prioritize them, and possibly decide which should be abandoned

- it should be memorable ( if it's going to be a big string of crap, we might as well just use raw ipv6s)
- it should be distributed  ( at least to the point that no one party can abuse their position, mutually assured destruction is a similar option, unless all parties have a common enemy to abuse (not good), all governments hate anarchists)
- it should be internal to the network (to whatever extent possible), not much point in having our own network if our browsing history gets sent to clearnet (and if clearnet fails, hype should keep going, hype is all about RESILIENCY)
- it should be simple for the user ( or else no one will use it, this means having a clear set of specs, so devs can work with different languages without dividing users)
  - it should somehow provide cryptographic proof of identity ( because if we don't have CAs, we need to somehow do it for ourselves)
  - maybe we should look into existing CA-alternative systems that have been proposed?
- it should integrate trust and allow the user to decide, again, NOT relying on a CA or other authority
- if possible, it should be based upon existing technologies to speed/ease adoption

I'd like to see a proposed flow of how the user would...

- install it ( this shouldn't take more than 5 commands, or even better, it should have a graphical installer or clickable script)
    - excluding compile times, of course
- use it in their browser ( it should NOT require any plugins, and should work identically in every browser, or from the command line (wget/curl))
    - so it needs system integration in some fashion
- sign up for/generate domains ( if signups are required, we should have a simple API or web interface ( hopefully in a language we all agree on (OR Language agnostic)))

Would it be possible to have it be a part of cjdns? eg one less step to use? // it kind of is, RAINFLYDNS, but the servers aren't running yet. We have 4 people running them, the system requires the cooperation of 13. If 8 others want to volunteer, I am willing to be the fifth. cjd gave his 'seal of approval'.

- That being said, conventional DNS (A LA hypeDNS and paraDNS) is not difficult to set up. I've detailed the way to do it http://couch.syrinxist.org/dns <- here for paraDNS
- When HypeDNS comes back online (supposedly on the weekend when finn isn't in school), I will add instructions for that.
- It's literally 3 commands (at least on Debian/ubuntu/mint/arch).
- I am willing to list instructions for OSX or other linux distros, but I am only able to test instructions on distros I run.
- If you figure out how to do it on anything else, let me know and I'll list it.

it was proposed (by prurigro) that an address be somehow based upon the owner's cjdns IPV6

- a 7 (or N) character string would be a part of your domain, and could not be spoofed.
- it would provide proof (perhaps optionally, like https) that a person is who they say they are (provided you know their IP, and actually bother to check)
- The issue with this is that if it is your TLD, then we don't have a memorable ending..
- in fact, no matter where it is, it probably isn't memorable, but it would be fairly secure, and would not depend on a certificate authority AT ALL.
- thusly, an address might look like: ansuz.lkj3ijg or kljdf23.ansuz
- might just be me, but these aren't memorable AT ALL
- I don't think it's just you. I'd only be interested if the crypto-hash were optional like https

this method could be offered IN ADDITION to the others. MAYBE:

If you lose your conf, though, you lose your domain, which kind of defeats the purpose of a domain as an abstraction from your raw address.

- cryptoProof.Owner.local.mesh
- ijdsgl2.ansuz.toronto.mesh ??
- ansuz.toronto.mesh # assuming you're confident you don't need the crypto-sign, like https (but without the heartbleed)

To what extent should it be linked to cjdns?

- having a dns module be an inherent part of cjdns means it should (in theory) be faster than alternatives.
- it would also be nice for it to come ready to run, having to install ANYTHING hinders adoption
- on the other hand, if the included dns is crap, everyone might use it just because it's there, then we'd all be using crap. that would suck.
- I'm in favour of modularity, but we need to consider all of our options IMHO

## jackv's DNS ideas

(just dumped here for lack of a better way of communicating them)

- The problem is not how we do DNS, it's how we know who to trust. Once we know that a node can be trusted, we can trust any traffic coming from it to be authentic, thanks to cjdns
  - Thus, we don't need to reinvent the wheel when it comes to DNS protocols, we just need to make cjdns smart enough to find trustworthy DNS servers
- In a global mesh, it's obviously hard for one host to serve all of the domains in the world
  - The idea about each meshlocal having a second level domain is good, thus "toronto.mesh" or "sandiego.mesh"
- Not everyone needs to run a DNS server, but it needs to be easy enough that anyone *could*, given the resources neeeded
  - Many (or at least a good number, proportional to size of meshlocal) DNS servers per meshlocal, but all serving the same records (ostensibly)
- The basic principle is that all DNS server nodes should be able to communicate and share records with each other fairly frequently, thus allowing DNS to propagate in a decent amount of time (nobody likes waiting for things like DNS propagation)
- All normal cjdns nodes should be able to communicate and share a list of active DNS servers to query
- A DNS server is set up for a single second level domain (eg sandiego.mesh), many DNS servers are set up for that same domain throughout the meshlocal
- A person can register a third level domain for an active cjdns address (eg jack.sandiego.mesh)
  - The relationship between domains and addresses should be one-to-one
  - The address should be queried on a regular basis. If it fails a certain number of times in a row, it should be labeled as defunct, and made available for open registration. This is done to accomodate small amounts of downtime, while making sure that every address is being maintained, preventing abuse
- A cjdns node should attempt to resolve a random sampling of domains for every name server that it knows about (obviously only .sandiego.mesh domains for a sandiego.mesh nameserver)
  - Each DNS server should probably serve a dump of all the domains in its database, maybe at the meta-domain "sandiego.mdns"
- Both the records that each nameserver serves and the nameservers that each node queries are based on whether a particular DNS node seems trustworthy
  - If the records it's serving are similar enough to the records of other DNS servers for the same "sandiego.mesh" range, then a node can be considerd trustworthy
- One of the biggest problems comes with the updating of domains, because this creates apparent inconsistencies (also, we don't want people hijacking a domain easily)

Possible Attacks

- Assuming that a majority is defined as 51%, then a 51% attack is a good way of attacking this structure, however, the term "majority" could also be defined as a higher numbe
In the bitcoin/cryptocurrency world, a pool with even around 40% could still do some damage
  - A minority attack would essentially just serve to impede the flow of accurate records, however the protocol should be smart enough to not get poisoned
- A node with very few peers could be fed a false list of nameservers, thus causing it to serve a bad list as well
  - This could be fixed by making name server information shared by more than just your peers
  - Also, if you're peering with bad nodes, DNS poison is only one of the problems you'll have
- One nameserver getting hacked isn't a huge deal, assuming there are a decent number of name servers for the same domain

Improvements

- Figure out how to update records
- Figure out how to use the crypto-proof idea (f4jl34m.jack.sandiego.mesh)



## discussion from hypeIRC/#webdev (edited and organized) // Kinda

```irc

<ansuz> finn and I were talking about setting up dns for every meshlocal
<ansuz> maybe changing .hype to .mesh
<ansuz> so like, pad.toronto.mesh
<ansuz> pad.seattle.mesh
<lukevers> pad.internet.mesh
<ansuz> rainflydns never happened
<lukevers> dns is hard

<ansuz> [parabolic] have 100% adoption of paraDNS
<ansuz> if we merge that with hype records, we might have something useful, I see no reason not to
<ansuz> and then eventually if we make something else, we can migrate all those records to it in one go (or just mirror them)
<ansuz> something distributed, maybe?
<qmx> lukevers: dns isn't hard, as long as you're fine with status quo
<qmx> (centralized)
<qmx> hypedns isn't viable
<lukevers> yeah centralized is kind of an easy concept

<qmx> rainfly won't fly

<lukevers> decentralized is more what I meant

<Igel> hey thats a kick in the nuts
<Igel> if its a 2 year+ think , but the way i saw it and it was described to me
<Igel> the docs distributed arent just quad records, they are arbitrary in his own words
<Igel> think of some examples, eg; bitcoin addy, gpg prints

<qmx> I still think we need to just have nodes advertising services over a dht discovery mesh
<Igel> #Hypertary
<Igel> workin on it
<Igel> (soon)
<qmx> and let gpg WOT assure I am qmx # WOT == web of trust

<ansuz> I'd really like it if we could just all use something, whatever it is
<ansuz> so that there's progress, instead of competition
<ansuz> not to say that they're mutually exclusive, but..
<ansuz> we shouldn't have to use 3 dns services to access all the records
<ansuz> I have more servers than I have things to host
<ansuz> I'll install whatever people think is best and help mirror it

<derp> we need open protocols, decentralizing everything only works if people actually use it. go lower, decentralize the procotol and let a user decide to self host or use a hosted service
<derp> ironically, its the chicken and egg problem. We want users, but users expect services. How do we attract and retain users? Making them host everything themselves?
<derp> the real beauty of cjdns is it solves network neutrality
<derp> assuming we don't impose some crazy HOCNET shit

<ansuz> I don't think everyone should have to serve dns records
<ansuz> we don't have to go 100% distributed
<prurigro> ansuz: so disapora-decentralized
<qmx> ansuz: if you use a dht you can have a lite dns client and some nodes host the full dns
<ansuz> but like, such that no one is in charge, I think that's what cjd was getting at with the nmc thing
<qmx> ansuz: akin to multibit-bitcoin and bitcoind
<ansuz> qmx, prurigro, yea

<ansuz> we should have a hype tld, .mesh, .hype or something
<derp> ansuz, cjd's nmc thing is way to hopeful. Its like he read Richard Stallmans dreams, but yeah, we need something *realistically better than the status quo*
<prurigro> qmx: when did he propose that? his earlier ideas were more like bitcoin, with power in numbers
<prurigro> (in terms of signage)
<prurigro> oh hm, yeah, that doesn't sound open
<qmx> prurigro: distributed but not that much
<prurigro> one person getting hit by a bus should not be the end of domain names
<qmx> prurigro: you would register via nmc
<qmx> prurigro: then his rainflydns signs the record
<ansuz> yea, that's silly
<ansuz> there's issues with nmc anyway
<ansuz> like changing records
<ansuz> centralization means you don't wait so long for things to propogate

<ansuz> local meshes should have something else
<ansuz> toronto.mesh
<ansuz> then toronto.mesh should issue ansuz.toronto.mesh
<ansuz> and host all the toronto records..

<derp> what about for the clearnet?
<derp> do you also plan to have a clearnet site for the meshlocal?
<ansuz> well, to attract new folks, you need a front porch
<ansuz> I use clearnet.syrinxist.org for that
<ansuz> but I'm gonna get something more obviously mesh related in the near future
<ansuz> starting to get toronto people somewhat involved this summer

<ansuz> I think this would be our best option, though
<ansuz> let local meshes deal with their own local users
<ansuz> if toronto.mesh goes offline, those people lose their records, unless they've made backups and are prepared to make the new service
<ansuz> (which should have a git or something)

<ansuz> centralization with redundancy is okay
<ansuz> I think
<ansuz> blockchains bring a lot of overhead

<qmx> ansuz: not for this thing
<qmx> if we can't have something fully decentralized, then just use standard DNS
<prurigro> I always thought a subscription model would make sense
<ansuz> that's what I'm saying
<ansuz> you want to use the toronto namespace? subscribe to torontoDNS

     ## my thought here is that ultimately we don't REALLY want to create services on hype for the whole planet to use..
     ## our primary concern should be our meshlocal users, since that's what project meshnet is really about.
     ## latency adds up over many hops, there's still black-holes..even if that goes away, there's bandwidth overhead to not browsing locally

<prurigro> people register with proejctmeshnet, projectmeshnet provides one gateway
<prurigro> they abuse it? drop them
<ansuz> torontoDNS feeds mirrors its toronto records to the global .mesh/.para/.hype
<prurigro> and everyone would register with a bunch of places to make sure one going currupt won't screw them over

<qmx> ansuz: I still strongly believe we should model for each node being able to issue his own namespace
<prurigro> kinda like drug dealers do on tor
<ansuz> qmx, I agree, but until we have that, I think this is our best option
<qmx> ansuz: we won't have that if we don't start building it
<ansuz> we could build it while we have this in the meantime
<ansuz> instead of competing over namespace
<qmx> ansuz: just the mention of .mesh/.para/.hype/.diarrhea sounds like a dirty hack
<ansuz> it's a better option than ICANN
<ansuz> which gets used more than anything else on hype
<ansuz> because there isn't a clear contender

<prurigro> another idea I had, was a tld of 4 utf8 characters that are cryptographically tied to a combination of the name they're attached to and the ipv6
<qmx> prurigro: make this 8 ascii chars
<qmx> utf8 for names are a pita generally
<prurigro> actually, what about 7
<qmx> whatever
<qmx> I like this idea a lot
<prurigro> the average number of items the brain can store in one go
<ansuz> I'm down
<prurigro> (people vary from 5-9)

<qmx> prurigro: tell me more about your idea
<ansuz> is there any way we could easily tie that into regular dns?
<ansuz> so we could roll it out without having to start from scratch?
<ansuz> premature optimization blows

<qmx> ansuz: I like cjd's idea of using .h as root
<ansuz> maybe we can start with some clear specs, and then it doesn't have to be tied to one language's implementation?
<ansuz> .h is fine by me
<ansuz> anything is fine, imho
<qmx> ansuz: gathering a set of reqs
<qmx> into a document

<qmx> then figuring out what can we build
<prurigro> qmx: well, if the ipv6 is 0:0:0:0:0 and the domain is prurigro, the tld will be 7 numbers generated cryptographically based on both of those values, which an extremely small number of other ipv6 addresses would be able to combine with the domain to form
<prurigro> kinda like how the ipv6 is cryptographically generated a much longer, but single source in cjdns
<prurigro> (the pubkey)
<prurigro> and a word + 7 characters is infinitely more remembreable than an ipv6
<qmx> so I could technically have blog.fcea93
<ansuz> don't you then have to change your domain if you change your cjdns conf?
<ansuz> change/lose
<qmx> ansuz: but then this is your problem, amirite
<ansuz> yea
<qmx> "don't be stupid"
<prurigro> and it'd be the number of ipv6 addresses dividided by 36^7 of all ipv6 addresses that would be able to spoof
<qmx> "friends don't let friends lose their private keys"
<ansuz> I've been good about it, but it would be nice if, say, all my nodes had one tld
<ansuz> easy to tell that *.ansuz all belongs to me
<qmx> I still think a blockchain with proof of work would be nice
<qmx> being the proof-of-work something that gives back to the network
<ansuz> harder to tell that it's me if anyone can make ansuz.235gdj
<qmx> ansuz: I agree

<qmx> prurigro: I think it's a must for one to be able to secure a domain
<qmx> and this is what makes everything harder
<ansuz> so what about meeting in the middle?
<ansuz> if we have tlds, but you have a crypto proof (optionally) in the subdomain?
<ansuz> lkjdsgj32.ansuz.toronto.mesh
<ansuz> or just ansuz.toronto.mesh
<ansuz> mesh is for all of hype, toronto is dependent on torontoDNS, I completely administrate *.ansuz.*
<qmx> ansuz: I still don't want a 3rd party controlling my domain names
<ansuz> I don't see how we can get around it unless you want all of your users to point to your own nameserver
<prurigro> qmx: yeah, the whole angle I was trying to think from when I came up with the idea I just suggested was in the hopes that dns could be almost as freeform as ipv6

<ansuz> but I agree, I'd prefer not to
<prurigro> there might be a better way to go about doing that, but this is the optimal solution imo
<prurigro> (someone can imagine up an domain and it simply belongs to them, because noone else could have it)
<ansuz> that's not too different from raw ipv6s, though
<prurigro> then, your packet bounces around until someone says they know "ansuz.285w92" + has the correct ip6
<ansuz> except they're shorter
<qmx> what about crossing gpg wot and names?

<prurigro> ansuz: it'd be verified against the ip6, and considered less secure but more convenient
<prurigro> as an option

<ansuz> i'm in favour of gpg
<qmx> which incidentally means if enough people sign I'm qmx I'm qmx

<ansuz> I'll sign ;)

<qmx> that's the thing
<qmx> you can have more than one entry for a record

<ansuz> signing is better than blockchain
<ansuz> for overhead, I would think

<qmx> I advertise I provide the qmx namespace, you can check all the signatures
<qmx> if a spoofer tries to provide qmx too, it's up to you to trust his
<qmx> since cjdns is built upon "know thy peers"

<ansuz> gets people to think more about gpg
<ansuz> and trust
<ansuz> which is good
<qmx> we start growing the web of trust

<ansuz> can we do this on top of an existing protocol?
<ansuz> since I think most people will agree that NIH is problematic
<qmx> I'm way more likely to trust something derp signed than something that (what was the name of the crazy one we trolled at #hyperboria?)
<qmx> ansuz: the client allows you to decide, and sign the records you trust

<qmx<unicode node glyph>
<qmx> note*
<ansuz> hehe, like my /whoami ?
<qmx> â™«
<ansuz> did you see that, qmx?
<ansuz> so, requirements..
<ansuz> it should be short
<qmx> example

<qmx> dig blog.qmx
<qmx> returns blog.qmx.chooser.h
<qmx> if you visit this it'll present you the records that match this tld and the sigs
<qmx> this page is provided by your dns client daemon
<qmx> which runs almost "inside" cjdns

<jackv> but the key is that you need to trust someone, once you have that, cjdns makes it possible to trust all the traffic coming from him as his own
<jackv> thus the problem here is finding someone (or multiple nodes) that you personally trust
<jackv> and "trust" could be defined as, their records agree with all 10 of the servers in my area
<jackv> a random sample of domains resolves the same with them as with a majority of the rest
<jackv> not everyone has to run one
<jackv> just make setting one up easy enough
<jackv> just like mail servers :)
```

## DNS, with trust based on peering-like agreements

Codename *lgdns*

```
<larsg> jercos: do you have any spontaneous thought re: web-of-trust-based dns on top of cjdns peers?
<jercos> larsg: cryptographic identities float right near the top of course, a response coming from an address is enough to trust that address as the source of the information contained therin
<jercos> uh
<larsg> it just seems so simple
<larsg> a mere ui problem
<jercos> yeah... seperating levels of trust is a thing though
<jercos> in an all-or-nothing trust system, the node with the highest trust level "wins" any conflicts, so your friend bob can send you a fake version of google.hype
<jercos> which means your friend bob is now a central point of failure for you, and you now trust that his machine is completely malware free
<jercos> ditto for *every* person you place at a higher level of trust than the original source for the google.hype that you expect to travel to
<larsg> just realize that what i'm thinking of is basically 1) a bit of glue around rainflyclient, and 2) rainflyserver on $every node
<larsg> yeah i get what you mean, you don't wanna roam into a rogue network and suddenly get fake answers
<larsg> if peers would be able to delegate the trust placed in them to other nodes, we could even do something similar to geoip-based dns
<larsg> not with coordinates as metric, obsly
<jercos> yup. possibly using DHT coordinates.
<larsg> yeah the server you're asking would delegate to another server (which it trusts) which is close to you
<larsg> instead of just blindly trusting every current peer, there could be a simple trust list of a few nodes
<larsg> and a means of seeding that list
<larsg> "You can't resolve domain names yet. Which of your peers or other nodes do you want to trust with resolution? [list of peers] [ipv6 address] [search]
<jercos> I'm headed homeward, I'll think on this on the drive :)
<larsg> safe ride!

<cjd> I've thought about some ideas like that
<larsg> i should spec that out a bit more, so that someone looking for work can understand it
<larsg> did you come with possible problems?
<cjd> I killed a few ideas off
<cjd> what if we just make a rule that we'll pull any changes to the db as long as they match the requirements and come from someone you trust
<cjd> requirements: can't hijack a domain which is already regged
<cjd> that's arguably a tragedy of the commons though since everyone will race to create new domains as fast as those connected to them will allow
<larsg> that description is a lot more concise, thnx
<cjd> in this context, trust is binary
<cjd> I add your node to the list -> I trust you absolutely
<cjd> I'll pull anything you offer
<cjd> like bgp
<larsg> or just for .com domains, or just for names matching /f.*ck/
<cjd> what's wrong with  firetruck.hype ?
<larsg> :]
<cjd> and why not match against flappingcuntlips ?
<cjd> or  throw-the-jew-down-the-well.hype
<cjd> if you're going to censor, you have a duty to do it right ;)
<larsg> de-peer
<larsg> err, untrust
<cjd> hmm
<cjd> the way I imagined it, there's no clear way to drop a domain once it's registered
<cjd> except for blocking it from being regged again and waiting for it to expire
<larsg> yeah but "registered" is relative to whom you trust
<cjd> meh that won't work
<cjd> you have to reach consensus
<larsg> y?
<cjd> b/c all (working) dns does
<cjd> if you rely on trust to determine what is the truth, you're going to have problems
<larsg> you only need your own local truth
<larsg> which you can make up of others' truths
<larsg> local communities could offer trust lists to their members
<larsg> and mutual trust could be established similar to how peering is established
<larsg> in terms of the social process
<larsg> mutual trust between communities, i mean.
<larsg> don't like what your community thinks about how .com should be resolved?
<larsg> tweak the community's default trust list
<cjd> ok so you're actually thinking of doing the whole internet
<larsg> cacert for dns i guess, with different tech underneath
<larsg> useless without the community around it
<larsg> just like cjdns
<cjd> well you could write some sort of a rules engine for defining the domains
<cjd> keep in mind though that it goes against how domains are normally registered
<cjd> err
<cjd> looked up
<larsg> most communities and individuals will want to resolve icann domains the icann way
<cjd> well you could write a rule like   .com -> 4.2.2.1
<cjd> .net -> 8.8.8.8
<cjd> penis.com -> 123.45.67.8   (if you reject how icann is handling it)
<cjd> or you could just say . -> fc00:.....friend
<cjd> hmm
<cjd> when a node spins up, if it has no dns config it could just pull from it's nearest peer
<larsg> or it'd "ask what do to do"
<cjd> asking is a sin
<larsg> yep indeed
<cjd> just search for the nearest node which is offering dns results
<larsg> FBI_Surveillance_Van_3
<larsg> or do you mean search in the dht?
<cjd> search the dht
<cjd> surveillence van will still fuck you, it just needs to be running cjdns
<larsg> that'd be a sensible default i guess
<cjd> but that's kind of the rule
<cjd> the van always wins
<cjd> unless you do a big public key signing shitfest
<larsg> if you're peering with someone over bluetooth (e.g. an android app that shares the .apk via bt), you'd likely also wanna take their dns
<larsg> so having multiple options there would useful
<larsg> AnotherInterface
<cjd> tor uses a list of 8 directory servers
<cjd> search the DHT for a directory server
<cjd> kind of rainfly method
<cjd> but directory servers die and stuff
<cjd> not so resilient
<larsg> on the peer level it's resilient though
<cjd> ehh zzz
<cjd> tomorrow
<larsg> k :)
<larsg> good night!
<larsg> thanks for the thoughts
<larsg> ... and so lgdns was born
```
