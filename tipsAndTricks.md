## Things nobody seems to know about cjdns
### (even those who have been using it for a very long time)

#### cjdns has an option to stay in the foreground

```Bash
./cjdroute --nobg < /path/to/cjdroute.conf
```

#### You don't need to run cjdroute as root

Comment the _router.interface_ section <!-- elaboration required --> of the conf and launch it like that. Your node will switch traffic, and peer effectively, though you will not be able to run services.

You have the option of configuring your TUN device manually. It will require root, but once established, cjdroute can otherwise run as an unprivileged user.



