# com.cjdelisle.cjdns.plist

LaunchDaemon for cjdns on OS X.

## Usage

Copy to /Library/LaunchDaemons

Set permissions and ownership with:

```
sudo chmod 644 /Library/LaunchDaemons/com.cjdelisle.cjdns.plist
sudo chown root:wheel /Library/LaunchDaemons/com.cjdelisle.cjdns.plist
```

Edit <string>/usr/local/bin/cjdroute</string> in the plist to match where you have cjdroute (/usr/local/bin/cjdroute is recommended).
Edit <string>/etc/cjdroute.conf</string> in the plist to match where you have the config file (/etc/cjdroute.conf is recommended).

Make sure config file /etc/cjdroute.conf exists and "noBackground":1 is set so cjdroute doesn't daemonize automatically.

Then load the plist with launchctl:

```
sudo launchctl load /Library/LaunchDaemons/com.cjdelisle.cjdns.plist
```