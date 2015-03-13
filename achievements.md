It has long been speculated that a file like this would be a great thing for Hyperborian newcomers to have, but no one has ever gotten around to writing it:

...behold!

# Hype Achievements

Apparently people don't do things just because they're a good idea. If keeping track of achievements via some arbitrary points system makes you feel better about following best practices, who am I to argue?

Each achievement will have an associated value. Your score is the sum of these points. See if you can get your score [over 9000](http://knowyourmeme.com/memes/its-over-9000)!

Achievements expire, so if at any given time you don't qualify, you lose those points!

1. Generate a PGP key
2. Install Git and create a Github account so you can submit Pull Requests and issues.
3. Build cjdns from source
4. Successfully peer with somebody.
5. Run only the latest version of cjdns on all your nodes.
6. Extra points for every node running the latest crashey branch.
7. `nmap` yourself and close down any exposed services (unless you intend for other people to use them).
8. Create a `~/.cjdnsadmin` file for use with the admin api.
9. Launch and maintain a webserver on Hyperboria.
10. Provide an RSS feed so others can subscribe for updates.
11. Help a newb on EFNet.
12. Read everything in [Project Meshnet's documentation repository](https://github.com/projectmeshnet/documentation).
13. `watch` the Documentation repository so you hear about updates.
14. Set up an IRC bouncer and point it at HypeIRC so you don't miss anything. Combinations of command line clients and tmux or screen are considered equivalent.
15. Contribute to the documentation repository at least once a month.
16. Participate in [keysigning Tuesday](#).
17. Make (or contribute to) a meshlocal page for your area.
18. Have a hype-enabled xmpp account
19. Configure a persistent link between nodes independent of the internet.
20. Blag about cjdns or Project Meshnet.
21. Design some form of rich media for Project Meshnet (graphics, video, animation, audio, presentation slides).
22. Discover a bug in cjdns, and give it a memorable name.
23. Host or register a cjdns, Project Meshnet, or Mesh-local mailing list.
24. Subscribe to any of the above lists.
25. Learn HTML
26. Have someone else quote you and commit the results to a HypeIRC bot's factoid system.
27. Build cjdns on an android phone.
28. Read all of [xkcd](http://xkcd.com/).
29. Install at least one Linux distribution other than Ubuntu.
30. Erase Microsoft windows from every computer you own.
31. Host your own email server.
32. Host your own ircd.
33. _Don't_ run your irc client as root!
34. Host a show on [HypeRadio](http://radio.cynical.us/hostashow.html).
35. Cross compile cjdns for another platform.
36. Contribute to [Hyperboria's news agency](http://news.hyperboria.net/).
37. Document an undocumented function or component of cjdns.
38. Implement a function or component of cjdns in an alternate language
39. Translate an article into another language (and maintain it).
40. Update cjdns without pinging out on IRC.
41. Configure an authorizedPassword without restarting cjdns.
42. Find out whether your home router can run cjdns: [OpenWrt table of hardware](http://wiki.openwrt.org/toh/start)
43. Try out the [Meshbox firmware](https://github.com/seattlemeshnet/meshbox) on your home router.

## Penalties

Avoid qualifying for these _achievements_, as they count against your score.

1. Provide peering credentials without including a means of getting in contact.
2. Provide peering credentials with extra information in the form of line or block comments. (You should be embedding them as JSON attributes instead!)
3. Provide peering credentials without the IPV6 included. (You can get by with just a password and publicKey, and your IPV6 can always be inferred from your publicKey, but it means more work to figure out who you are).


## You may have noticed some patterns...

* **You need to pay attention to your nodes**. A network is an inherently social construction. Out of date, buggy nodes can actually have quite a negative effect on the nodes around them. Update often, and keep track of how different versions perform, we need feedback!
* **RSS, mailing lists, and other subscription protocols are really valuable**! Improving this software (and the network built on top of it) means gathering feedback. You can't respond to questions unless you are first aware of them.
* **Source code is always better**. At this point in time, the most authoritative definition of the cjdns protocol is [cjd's github repository](https://github.com/cjdelisle/cjdns). If you are using another source, it is far more likely to be out of date.
* **Newest is best**. Every now and then there are intentionally breaking changes. This happens when the network is suffering because of old nodes. In such cases, modifications are made which cause up to date nodes to drop old nodes' traffic. If you don't update, you might fall off the map. If you are running a protocol in between the cutoff point and the bleeding edge, you may be the link which allows older nodes to continue participating in the network. Please update so we can all use the latest features to better diagnose bugs.
* **There is no substitute for understanding**. People build tools that streamline difficult processes, but ultimately you cannot rely on software to fix all of your problems. At some point, bad behaviour has to change, and that means understanding the principles behind security, exercising discipline, and informing those around you when they are putting themselves (and possibly others) at risk.
* **We cannot rely entirely on the experts**. This is closely related to [Brooks' Law](http://en.wikipedia.org/wiki/Brooks%27s_law). An expert in a subject is in an excellent position to push further, and learn those things which are out of reach of those with less experience in the subject. Unfortunately, this often means they are in the position of having to choose between learning more about the subject in question, and spending their time sharing disseminating their knowledge. It is very important to understand that when people take the time to help you understand a difficult subject, they need you to help share that information with those who know are less experienced than you. In the Hypeborian community, we've taken to referring to this method as [WTFM](http://www.roaming-initiative.com/blog/posts/wtfm).


