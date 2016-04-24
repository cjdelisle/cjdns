cjdroute(1) -- Cjdns packet switch
=============================================

## SYNOPSIS

`/usr/sbin/cjdroute` [<options>...]

## DESCRIPTION

Cjdns implements an encrypted IPv6 network using public-key cryptography for
address allocation and a distributed hash table for routing. This provides
near-zero-configuration networking, and prevents many of the security and
scalability issues that plague existing networks.

cjdroute runs in the background and either decrypts packets addressed to
this node, or sends them on to the next node in the route.

##OPTIONS

  * `--help`:
    Print usage summary.

  * `--genconf` [--no-eth]:
    Generate a configuration file, write it to stdout.
    If --no-eth is specified then ethernet beaconing will be disabled.

  * `--bench`:
    Run some cryptography performance benchmarks.

  * `--version`:             
    Print cjdroute version and the protocol version which this node speaks.

  * `--cleanconf` < conf:
    Print a clean (valid json) version of the config.

  * `--nobg`                
    Never fork to the background no matter the config.

##USAGE

To get the router up and running:

  * Step 1:
    Generate a new configuration file if one doesn't already exist.

      cjdroute --genconf > /etc/cjdroute.conf

    You can also simply:

      systemctl start cjdns

    which will tell cjdroute to generate a new config if needed.

  * Step 2:
    Find somebody to connect to.
    Check out the IRC channel or http://hyperboria.net/
    for information about how to meet new people and make connect to them.
    Read more here: https://github.com/cjdelisle/cjdns/#2-find-a-friend

    By default, cjdroute will find any cjdns nodes on your local LAN
    without any configuration.

  * Step 3:
    Add that somebody's node to your cjdroute.conf file.
    https://github.com/cjdelisle/cjdns/#3-connect-your-node-to-your-friends-node

  * Step 4:
    Fire it up!

      systemctl start cjdns

    Or if you had already started cjdns:

      systemctl restart cjdns

For more information about other functions and non-standard setups, see README.md
