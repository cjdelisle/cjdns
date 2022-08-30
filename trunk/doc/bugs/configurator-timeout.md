# Configurator.c: Timed out waiting for a response

In some scenario cjdns could fail at starting up with the following cryptic
messages:

```
1428440513 DEBUG AdminClient.c:333 Connecting to [127.0.0.1:11234]
1428440513 DEBUG UDPAddrIface.c:294 Bound to address [0.0.0.0:58063]
1428440518 CRITICAL Configurator.c:96 Failed to make function call [Timed out waiting for a response], error: [ping]
1428440518 CRITICAL Configurator.c:56 enable Log_LEVEL=KEYS to see message content.
1428440523 CRITICAL Configurator.c:68 Failed to stop the core.
1428440523 CRITICAL Configurator.c:70 Aborting.
```

## Cause

cjdns is configured via the admin interface, listening by default on
`localhost`, port `11234`.

However, there are some machine configurations where the firewall has a default
policy different from ACCEPT for the INPUT chain and there's an exception for
loopback interface missing. Checking with `iptables -t filter -L` (as root)
should output something like:

```
Chain INPUT (policy ACCEPT)
target     prot opt source               destination

Chain FORWARD (policy ACCEPT)
target     prot opt source               destination

Chain OUTPUT (policy ACCEPT)
target     prot opt source               destination
```

The above output is the default configuration of the firewall, with everything
allowed, your own could be configured in many different ways. However, you have
to check your INPUT chain policy, if it's REJECT or DROP that could be causing
the problem above.

## Solution

There are a couple of solutions to this issue.

### ACCEPT everything on INPUT

Piece of cake: just run `iptables -t filter -P INPUT ACCEPT`.

### Add an exception for loopback traffic

This solution is more complex, but allows you to retain your full firewall
functionality while allowing cjdns at the same time.
You have to allow both inbound and outbound traffic on the loopback interface,
usually named `lo`. That name is the one which is going to be used in the
following code snippet.

```
iptables -t filter -I INPUT -i lo -j ACCEPT
iptables -t filter -I INPUT -o lo -j ACCEPT
```

Ok, I cheated. The rules are really simple: just ACCEPT everything from (`-i`) or
to (`-o`) loopback interface (`lo`) on chain `INPUT`.

Maybe not so simple really...

#### ... and now you can't peer

Just adding an exception for `lo` is not enough if you want to peer via UDP and
you didn't load the conntrack module. Load it or add an exception for the port
used by the UDPInterface.

Or just use the first solution.

