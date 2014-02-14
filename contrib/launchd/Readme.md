# com.cjdelisle.cjdns.plist

LaunchDaemon for cjdns on OS X.

## Usage

Copy to /Library/LaunchDaemons

Set permissions and ownership with:

```
sudo chmod 640 /Library/LaunchDaemons/com.cjdelisle.cjdns.plist
sudo chown root:wheel /Library/LaunchDaemons/com.cjdelisle.cjdns.plist
```

Make sure cjdroute is in /usr/local/bin
Make sure config file /etc/cjdroute.conf exists and "noBackground":1 is set so cjdroute doesn't daemonize automatically.

Then load the plist with launchctl:

```
sudo launchctl load /Library/LaunchDaemons/com.cjdelisle.cjdns.plist
```