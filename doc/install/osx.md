# Installing on Mac OS X

The easiest way to get cjdns is to use [brew](http://brew.sh/). To get the latest brew formulae run this command from your terminal:

`brew update`

Then you should be able to install cjdns with by running:

`brew install cjdns`

After which you should have a cjdroute binary available at `/usr/local/Cellar/cjdns/<version>/bin/cjdroute`. Usually brew will link the binary to your `/usr/local/bin directory`, to make it available in your `$PATH`.

# Running CJDNS as launchd daemon  
To keep cjdns running in background and to start it automatically when your Mac turns on, it is possible to create a launchd daemon.

First of all we need to create a script that will be called by launchd and run cjdroute with the specific configuration. This file will contain the path of your configuration, for this example I will assume that your settings are stored in `/etc/cjdroute.conf`. It is vital that the service will NOT run in background, so remember to edit the end of the configuration to have something similar to this:

```
...

    // If set to non-zero, cjdns will not fork to the background.
    // Recommended for use in conjunction with "logTo":"stdout".
    "noBackground": 1
}
```

Create or/and edit the file `/usr/local/bin/cjdroute_start` with the following content:

```
#!/bin/bash
# You can use custom path or enable sleeping time to delay the process
/usr/local/bin/cjdroute < /etc/cjdroute.conf
```

Now that the script is created, we need to write the instructions for launchd. Create the file `/Library/LaunchDaemons/com.cjdns.cjdroute.plist` using administrator permissions (`sudo nano ...`) and insert the following content in it:

```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN"
    "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>Label</key>
        <string>com.cjdns.cjdroute</string>
    <key>Program</key>
        <string>/usr/local/bin/cjdroute_start</string>
    <key>RunAtLoad</key>
        <true/>
    <key>KeepAlive</key>
        <true/>
</dict>
</plist>
```

This will keep our process alive and run it at the startup. At this point we just need to set the right permissions and load the daemon in the configuration by running the following commands in a termianl:

```
sudo chmod +x /usr/local/bin/cjdroute_start # Execution
sudo sudo launchctl load /Library/LaunchDaemons/com.cjdns.cjdroute.plist 
```

If everything is correct, you can now restart your Mac and it will automatically run cjdns. If you want to avoid the reboot, you can start the daemon using the following command:

```
sudo launchctl start com.cjdns.cjdroute # To start without rebooting
```

Enjoy!
