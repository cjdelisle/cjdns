## cjdns connectTo buffer overflow bug

I (and [several others](https://github.com/cjdelisle/cjdns/issues/701)) experienced an issue where cjdroute would fail to launch, with the following error:

```
...
1423794312 INFO cjdroute2.c:560 Forking angel to background.
1423794312 DEBUG Pipe.c:135 Buffering a message
1423794312 DEBUG cjdroute2.c:597 Sent [204] bytes to angel process
1423794312 DEBUG Pipe.c:232 Pipe [/tmp/cjdns_pipe_client-angel-yubd7j5m8vvjn3fju1nvw47v316g05] established connection
1423794312 DEBUG Pipe.c:254 Sending buffered message
1423794312 DEBUG AdminClient.c:333 Connecting to [127.0.0.1:11234]
1423794312 DEBUG UDPAddrInterface.c:293 Bound to address [0.0.0.0:36018]
1423794317 CRITICAL Configurator.c:96 Failed to make function call [Timed out waiting for a response], error: [UDPInterface_beginConnection]
1423794317 CRITICAL Configurator.c:56 enable Log_LEVEL=KEYS to see message content.
1423794317 CRITICAL Configurator.c:70 Aborting.
```

## The cause

After some trial and error, I narrowed the cause of the factor down to a particular block in the `connectTo` section of my configuration file.

```
"192.168.10.102:6447":
{
"password":"anArbitraryPasswordOfThisLength",
"publicKey":"3kcy5s4fvp6f1tzx2f9llm0dp19y4xz1z9t1rftf45103s2b7670.k",
"operator":"aLargeNumberOfAliasesforAUser",
"contact":"aNodeOperator@anEmail.tld",
"location":"aVerboseDescriptionOfAPhysicalLocation",
"ipv6":"fc00:0000:0000:0000:0000:0000:0000:0000"
}
```

At first I commented out the `"ipv6"` line, and suddenly my node was able to launch. Using **pigeon logic**, I figured that perhaps "ipv6" had some special significance in an outgoing connectTo block. I tried making it "IPV6" instead, and padded the ipv6 with missing zeros to bring it up to the standard 39 character representation. Nothing worked.

At this point, I just commented out the ipv6 field, and relaunched the node. It worked, and for a while I was happy enough with that solution, but I wanted to understand what was happening.

I asked [cjd](http://github.com/cjdelisle/) and he suggested commenting out a different field instead. Surely enough, commenting out any member of the data structure (other than those that are required by the cjdns angel) was enough to make the configuration file valid.

So as it turns out, the error occurred as a result of the length of the bencoded packet being sent to the angel exceeding the hard limit of the length of a UDP packet. Again, at **cjd**'s suggestion, I used a commonly available tool to diagnose the issue:


## How to do it yourself

**excerpted from IRC logs**

```
10:49 <@__cjd__> did you tcpdump lo ?
10:49 <@__cjd__> tcpdump -A -s0 -i lo
```

I personally ran `sudo tcpdump -A -s0 -i lo > connectTo.overflow.log`, then attempted to launch cjdns using my `buggy.conf`.

## What the traffic looks like

```
E..'..@.@.............+....'d0000006:cookie10:13904808324:hash64:723280ffa58bd47ce5dad1f42199fe3d3ff73c4e9fd6859d342b872f31c848ab1:q4:auth2:aq28:UDPInterface_beginConnection4:argsd7:address19:192.168.10.102:644715:interfaceNumberi0e4:ipv639:fc00:0000:0000:0000:0000:0000:0000:00008:location38:aVerboseDescriptionOfAPhysicalLocation7:contact25:aNodeOperator@anEmail.tld8:operator40:anEspeciallyVerboseListOfAliasesForAUser9:publicKey54:3kcy5s4fvp6f1tzx2f9llm0dp19y4xz1z9t1rftf45103s2b7670.k8:password31:anArbitraryPasswordOfThisLengthe4:txid8:07000000e
13:32:52.313755 IP localhost.11234 > localhost.34563: UDP, length 28
E..8..@.@...........+....$.7d5:error16:Request too big.e
```

## A successful conf

I shortened this `connectTo` block one character at a time until `cjdroute` launched successfully. This block is the maximum length that passes. 

```
"192.168.10.102:6447":
{
  "password":"anArbitraryPasswordOfThisLength",
  "publicKey":"3kcy5s4fvp6f1tzx2f9llm0dp19y4xz1z9t1rftf45103s2b7670.k",
  "operator":"a_ShorterListOfAliasesForAUser",
  "contact":"aNodeOperator@anEmail.tld",
  "location":"aVerboseDescriptionOfAPhysicalLocation",
  "ipv6":"fc00:0000:0000:0000:0000:0000:0000:0000"
}
```

The `bencode`d form is displayed below:

```
E.....@.@.yS.........X+.....d6:cookie10:13904809584:hash64:7f050f5968df6d94c2cd044625e00b10d066555eee5bb5fbdc5f763e256126891:q4:auth2:aq28:UDPInterface_beginConnection4:argsd7:address19:192.168.10.102:644715:interfaceNumberi0e4:ipv639:fc00:0000:0000:0000:0000:0000:0000:00008:location38:aVerboseDescriptionOfAPhysicalLocation7:contact25:aNodeOperator@anEmail.tld8:operator30:a_ShorterListOfAliasesForAUser9:publicKey54:3kcy5s4fvp6f1tzx2f9llm0dp19y4xz1z9t1rftf45103s2b7670.k8:password31:anArbitraryPasswordOfThisLengthe4:txid8:07000000e
13:35:01.776419 IP localhost.11234 > localhost.53848: UDP, length 36
E..@..@.@.{)........+..X.,.?d000005:error4:none4:txid8:07000000e
```

## Conclusion

Keep your connectTo sections short! It's useful to have an ipv6, a contact, and a nick for a user. If you want to add a "location" field, add it in an abbreviated form, like `"location":"Toronto,CA"`.
