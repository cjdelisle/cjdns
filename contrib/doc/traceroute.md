cjdns-traceroute(1) -- trace cjdns packet routing
=============================================

## SYNOPSIS

`cjdns-traceroute` <host_or_ip>

## DESCRIPTION

Because cjdns is end to end encrypted, the standard traceroute always
shows a direct connection.  Cjdns-traceroute queries cjdroute to discover
what route would be used to send a packet to the destination IP.
Note that just as with the standard traceroute, there is no guarantee
that that precise route will actually be used with the next packet.

Cjdns-traceroute requires admin privilege.  The cjdnsadmin lib will 
try to read `/etc/cjdroute.conf` for the admin password.  This will 
normally succeed only for root.  Otherwise, it tries `~/.cjdnsadmin`

##USAGE

##FILES

`~/.cjdnsadmin`
`/etc/cjdroute.conf`

##BUGS
Cjdns-traceroute throws an ugly exception if you forget
to pass an IP or have the wrong admin password.

## SEE ALSO

cjdroute(1)
