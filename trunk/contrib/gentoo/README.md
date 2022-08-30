# Gentoo

## Automatic crash detection and restart

Copy the the openrc init script from `contrib/openrc` to `/etc/init.d/` and modify the `CONFFILE` and `command` parameter to your needs. 
Then start cjdns by issuing

    /etc/init.d/cjdns start

Configure the init system to autostart cjdns

    rc-update add cjdns default

Copy the service_restart script `contrib/gentoo/service_restart.sh` to any convenient directory on 
your system and modify the eMail address. If you do not wish to be notified, comment out the whole line.
Now add an crontab entry like this

    # Restart crashed Services
    * * * * *       root	/path/to/script/service_restart.sh
