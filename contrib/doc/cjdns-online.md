cjdns-online(1) -- check whether cjdns tunnel devices are available
=============================================

## SYNOPSIS

`cjdns-online` [<options>...]

## DESCRIPTION

Cjdns-online waits for cjdns to make its tunnel device available so 
that services that listen on the cjdns IP can start.  If you have
configured services to listen on the cjdns IP, then you can use:

    systemctl enable cjdns-wait-online

to wait until this IP is available before starting network services.
This will increase your boot time somewhat, but is needed to launch
thttpd at boot, for example.

##OPTIONS

  * `-t`, `--timeout` <timeout_value>:	
    time to wait in seconds, default 30

  * `-x`, `--exit`:		
    Exit immediately if cjdns is not online

  * `-q`, `--quiet`:
    Don't print anything

  * `-s`, `--wait-for-startup`:
    Wait for full startup instead of just tun dev.  This is not implemented.

