# CJDNS Android #

## BUILD CJDNS for Android ##

### Dependencies ###

* **Git**: Required to clone the git repo.
* **Node.js**: Required to run the build script.
* **Python 2**: Required to build a dependency during the build process.
* **Android NDK** (optional): Required to compile the project for Android (will be downloaded if not available.)

### Build Instructions ##

1. **Configure Location of Android NDK** (optional): If you already have the Android NDK installed, set the _$NDK_ variable in your shell to the path of the folder where it's located. (eg: `export NDK="/opt/android-ndk"`)
2. **Configure Alternative Repository** (optional): To use an alternative repository, open _cjdroid-build.sh_ and edit _REPO=https://github.com/cjdelisle/cjdns.git_ to point to the URL of your desired repository. (eg: _REPO="http://newrepo"_)
  * **NOTE**: You must remove the _cjdns-android/cjdns_ folder if it exists after changing the repo before you can run _cjdroid-build.sh_ again.
3. **Configure Alternative Branch** (optional): To use a branch other than _master_, open _cjdroid-build.sh_ and edit _BRANCH="master"_ to use the name of your desired branch. (eg: _BRANCH="newbranch"_)
4. **Build**: Run _cjdroid-build.sh_ and wait until it completes.

## Deploy CJDNS for Android ##

### Requirements ###

* **Root**: This is required to install things to the _/system_ partition as well as configure the network. The method required to root a phone differs from model to model.  If your phone isn't rooted yet and you're not sure where to start, look for the subforum for your device on XDA forums (linked below), and hopefully you'll find something that works.
* **Tun Support**: This provides the interface cjdns uses to create a virtual network device. Most (if not all) 4.0+ phones include tun support. If yours uses 2.x, CyanogenMod and some stock ROMs include support, but many don't. If your phone doesn't have a TUN device at /dev/tun, download [com.aed.tun.installer](http://cjdns.ca/com.aed.tun.installer.apk), then install and run it to have it set one up for you.

### Setup Instructions ###

After building _cjdroute_, the _cjdroid-build.sh_ script will generate a compressed file named something like _cjdroid-version.tar.gz_ (eg: _cjdroid-0.3.2380.tar.gz_.)

1. Copy the compressed file to your Android device and extract it.
2. Become root by running: `su`
3. Execute the install script by running: `sh cjdroid/install.sh`
3. Create a directory in _/sdcard_ named _cjdns_ by running: `mkdir /sdcard/cjdns`
4. Either copy a premade cjdroute.conf file to _/sdcard/cjdns/cjdroute.conf_, or create a new one by running: `cjdroute --genconf > /sdcard/cjdns/cjdroute.conf`
5. Add peers to the connectTo section of _/sdcard/cjdns/cjdroute.conf_ if necessary.
6. Run `cjdctl start` to start _cjdaemon_ and _cjdroute_.
7. Test to make sure everything is working:
  * **Check Configuration**: Check that your tun device (usually tun0) was configured correctly by running: `ip addr`
  * **Check Process**: Ensure cjdroute is running with the following command: `ps | grep cjdroute`
  * **Check Config**: Starting _cjdroute_ manually by running `cjdroute < /sdcard/cjdns/cjdroute.conf` should output to stdout, which you can use to help debug configuration issues.
  * **Ping Test**: Find the ipv6 of one of your peers and run `ping6 ipv6address`
  * **Admin Port**: You can configure the _admin_ section in your cjdroute.conf to bind to the ipv4 of your local network, then connect using the tools included in the _contrib_ folder to help debug issues.
  * **NOTE**: Remember that _/sdcard/cjdns/cjdroute.conf_ must exist for _cjdctl_ or _cjdaemon_ to start _cjdroute_.

### Commands ###

Once _install.sh_ is run and _/sdcard/cjdns/cjdroute.conf_ exists, _cjdctl_ can be run as root from the commandline with the following options:

* **cjdctl start**: Run cjdroute and the daemon, and tell the daemon to start cjdroute at runtime and when the phone wakes (this state will continue after restarting.)
* **cjdctl stop**: Stop cjdroute and tell the daemon not to start cjdroute at runtime or when the phone wakes (this state will continue after restarting.)
* **cjdctl restart**: Run the stop and start commands above, in that order.
* **cjdctl start-daemon**: Start the daemon if it isn_t running, which will then start cjdroute on init and when the phone wakes based on the state set by the most recent start/stop command, and stop cjdroute when the phone sleeps.
* **cjdctl stop-daemon**: Stop the daemon if it's running, preventing cjdroute from being stopped when the phone sleeps and started when it wakes.
* **NOTE**: Once cjdns starts successfully after running `cjdctl start`, it will automatically start on boot and stop/start on sleep/wake until the `cjdctl stop` is run.

### Files ###

Below is a list of the files contained in _cjdroid-version.tar.gz_:

* **install.sh** (run by user as root): This remounts _/system_ read/write and installs everything in the _files_ folder in their respective locations.
* **files/99cjdroute** (run by system): Deployed to _/system/etc/init.d/99cjdroute_, this is run at boot to start _cjdaemon_.
* **files/cjdaemon** (run by system): Deployed to _/system/bin/cjdaemon_, this starts cjdroute if it's supposed to be running, then stops it when the phone sleeps and starts it again if it's supposed to be running when the phone wakes back up.
* **files/cjdctl** (run by user as root): Deployed to _/system/bin/cjdctl_, this lets you control cjdroute as well as the daemon.
* **files/cjdroute** (run by user as root and system): This is the _cjdroute_ binary, which can be used like normal to generate a config file with: `cjdroute --genconf > /sdcard/cjdns/cjdroute.conf`, and you can run: `cjdroute < /sdcard/cjdns/cjdroute.conf` to start it manually (running it manually may result in higher than normal battery drain in sleep mode).
* **NOTE**: The files included in _cjdroid-version.tar.gz_ are from the repo cloned by _cjdroid-build.sh_ @  _cjdns-android/cjdns_ (and not the current repository, if you're running a copy of _cjdroid-build.sh_ from inside one.)
