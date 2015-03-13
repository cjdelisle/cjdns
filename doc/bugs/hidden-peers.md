# The Hidden Peers Bug

## THIS BUG HAS BEEN FIXED AS OF V14 ##

This is a class of bugs where direct peers

- fall out of the NodeStore (i.e. the routing table)
- make it into the NodeStore delayed by seconds to minutes
- never make it into the NodeStore at all

If you have only one peer, the result of this obviously is totally broken
routing. In rare cases, packet forwarding works, and you can still (to some
degree) establish connectivity to the peer without it being in the NodeStore.

With multiple peers, the result might be less bad -- needs more information.


## Diagnostics

Assuming your peer's IP address is `fc67:9816:2ccc:c4c2:f76c:1d09:a7a5:044e`,
you can use the following tools to diagnose the Hidden Peers bug.

```
$ contrib/nodejs/tools/peerStats.js | grep 044e
fc67:9816:2ccc:c4c2:f76c:1d09:a7a5:044e  0000.0000.0000.0015  in 1844334  out 1012270  ESTABLISHED  dup 0 los 6 oor 0

$ contrib/nodejs/tools/dumptable.js | grep 044e
fc67:9816:2ccc:c4c2:f76c:1d09:a7a5:044e 0000.0000.0000.0015 199687481 13 76
```

If you are hitting the bug, peerStats.js looks just like above, but the output
of dumptable.js doesn't include the peer.

In some cases, dumptable.js does include the peer, but the path reads `ffff.ffff.ffff.ffff`.
This is likely the case shortly before the peer falls out of the NodeStore, right
after the link has turned bad, for whatever reason.


# Collecting logs

On both ends, e.g. my laptop and the VPS it's peered with, I collect logs
related to the respective other node.

```
$ sudo gdb ./cjdroute -ex 'set follow-fork-mode child' -ex 'run < /etc/cjdroute.conf' -ex 'thread apply all bt' -ex 'quit' &> gdb.log
# In another shell
$ tail -f gdb.log | grep -P '044e|vsbxsthgml9l7wxyqn1b9nc2c9cucbmdps0fh3gd2jn9ys017590'
```


# Underlying issues

Maybe something related to SwitchPings. The peering gets established, but in
order for the peer to be added to the NodeStore, we need to find out its
protocol version, which is why we send a SwitchPing.
