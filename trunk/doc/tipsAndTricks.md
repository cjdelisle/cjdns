## Things nobody seems to know about cjdns
### (even those who have been using it for a very long time)

#### cjdns has an option to stay in the foreground

```Bash
./cjdroute --nobg < /path/to/cjdroute.conf
```

#### You don't need to run cjdroute as root

Comment the _router.interface_ section <!-- elaboration required --> of the conf and launch it like that. Your node will switch traffic, and peer effectively, though you will not be able to run services.

You have the option of configuring your TUN device manually. It will require root, but once established, cjdroute can otherwise run as an unprivileged user.

#### Lint the configuration using JSHint/jsonlint

This is a little trick that will lint the configuration file (`cjdroute.conf`) before starting cjdns.

##### JSHint
Will allow comments, note that JSHint is designed for JS and may not display errors and warnings etc. in all cases.
```Bash
jshint ./cjdroute.conf; if [[ $? == 0 ]]; then ./cjdroute < ./cjdroute.conf; fi
```

##### jsonlint
No comments or other JS exclusive object quirks will be allowed.
```Bash
jsonlint ./cjdroute.conf; if [[ $? == 0 ]]; then ./cjdroute < ./cjdroute.conf; fi
```
