Those familiar with **cjdns** have likely heard others refer to a **black hole routing bug**. This is one of the central problems we have to solve. Thus far, we have had a hard time fixing this issue because we don't fully understand what is causing it.

Firstly, it's quite possible that we are referring to an apparent behaviour which is actually caused by a number of different bugs. For that reason, it may be helpful to use the term to refer to the behaviour, and find other terms to refer to the actual causes, once we understand them.

The bug is so named partially because it is so difficult to diagnose (actual black holes cannot be seen directly), and partially because the end result is that information seems to disappear (black holes absorb light).

The simplest possible network configuration in which the bug can arise is in a **chain** topology. Suppose Alice connects to Bob, and Bob to Charlie. Situations have arisen in which Alice is able to connect to Charlie, and yet unable to communicate directly with Bob. In such a situation, it's obvious that something strange is happening. It becomes less obvious as the network grows.

If you find that you are unable to route to a peer that you believe you should be able to reach, you can help us solve this bug by collecting as much information as possible about your node at the time:

Create a bug report:

```bash
# what version are you using?
cd cjdns;
./cjdroute -v > ~/cjdns-bug-report.txt

# more specifically, which git commit?
git rev-parse HEAD >> ~/cjdns-bug-report.txt

# let us know your cjdns ipv6 address
ip a | grep " fc" >> ~/cjdns-bug-report.txt
```

```bash
# Dump your routing table to a text file:
cd contrib/nodejs/tools;
./dumptable.js >> ~/cjdns-bug-report.txt

# peerStats might be helpful too
./peerStats.js >> ~/cjdns-bug-report.txt
```

Add some comments to your bug report telling us about the problem in as much detail as you can think of.

```Bash
$EDITOR ~/cjdns-bug-report.txt
```

Post it, or host it, and get on IRC!
