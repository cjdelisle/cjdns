peerStats(1) -- show cjdns peers
=============================================

## SYNOPSIS

`peerStats`

## DESCRIPTION

Cjdroute talks to a number of immediate peers configured in
`/etc/cjdroute.conf`.  These can be discovered dynamically on local
networks if "beacon" is enabled.  Peers are enabled to connect
to cjdroute by adding a login in "authorizedPasswords".  Connecting
out to peers is configured in "connectTo" in either the IPv4 or IPv6 section.

No admin privilege is needed to run peerStats, but it looks in `~/.cjdnsadmin`
for the IP and admin port of cjdroute.

##FILES

`~/.cjdnsadmin`

## SEE ALSO

cjdroute(1), sessionStats(1)
