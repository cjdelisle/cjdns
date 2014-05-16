# CJDNS Android #

## BUILD CJDNS for Android ##

1. **Install Dependencies** (optional): Building cjdns requires the following dependencies:
  * git (to clone the repo)
  * node.js (to run the build script)
  * python2 (used by the build script)
2. **Configure NDK** (optional): If you already have the Android NDK installed, set the _$NDK_ variable in your shell to the path of the folder where it's located. (eg: `export NDK="/opt/android-ndk"`)
3. **Configure Repo and Branch** (optional): To use a repo other than CJD's Github or a branch other than _master_, open _cjdroid-build.sh_ and edit the _$REPO_ or _$BRANCH_ variables inside accordingly. (eg: _REPO="http://newrepo"_ -or- _BRANCH="newbranch"_)
  * **Note**: You must remove the _cjdns-android/cjdns_ folder if it exists after changing the repo before you can run _cjdroid-build.sh_ again.
4. **Build**: Run _cjdroid-build.sh_ and wait until it completes.
5. **Locate Binary** (optional): The resulting binary should be located @ _cjdns-android/cjdns/cjdroute_

## Deployable System for Android ##

### Deployment ###

After building _cjdroute_, the _cjdroid-build.sh_ script will generate a compressed file named something like _cjdroid-version.tar.gz_ (eg: _cjdroid-0.3.2380.tar.gz_.)

1. Copy this file to your Android device and extract it.
2. Become root by running: `su`
3. Execute the install script by running: `sh cjdroid/install.sh`
3. Create a directory in _/sdcard_ named _cjdns_ by running: `mkdir /sdcard/cjdns`
4. Either copy a premade cjdroute.conf file to _/sdcard/cjdns/cjdroute.conf_, or create a new one by running: `cjdroute --genconf > /sdcard/cjdns/cjdroute.conf`
5. Add peers to the connectTo section of _/sdcard/cjdns/cjdroute.conf_ if necessary.
6. Run `cjdctl start` to start _cjdaemon_ and _cjdroute_.
7. Test to make sure everything is working:
  * Find the ipv6 of one of your peers and run `ping6 ipv6address`
  * Check that your tun device (usually tun0) was configured correctly by running: `ip addr`
  * Ensure cjdroute is running with the following command: `ps | grep cjdroute`
  * Keep in mind that _/sdcard/cjdns/cjdroute.conf_ must exist for _cjdctl_ or _cjdaemon_ to start _cjdroute_.

### Files ###

Below is a list of the files contained in _cjdroid-version.tar.gz_:

* **install.sh** (run by user as root): This remounts _/system_ read/write and installs everything in the _files_ folder in their respective locations.
* **files/99cjdroute** (run by system): Deployed to _/system/etc/init.d/99cjdroute_, this is run at boot to start _cjdaemon_.
* **files/cjdaemon** (run by system): Deployed to _/system/bin/cjdaemon_, this starts cjdroute if it's supposed to be running, then stops it when the phone sleeps and starts it again if it's supposed to be running when the phone wakes back up.
* **files/cjdctl** (run by user as root): Deployed to _/system/bin/cjdctl_, this lets you control cjdroute as well as the daemon.
* **files/cjdroute** (run by user as root and system): This is the _cjdroute_ binary, which can be used like normal to generate a config file with: `cjdroute --genconf > /sdcard/cjdns/cjdroute.conf`, and you can run: `cjdroute < /sdcard/cjdns/cjdroute.conf` to start it manually (running it manually may result in higher than normal battery drain in sleep mode).

### Commands ###

Once _install.sh_ is run and _/sdcard/cjdns/cjdroute.conf_ exists, _cjdctl_ can be run as root from the commandline with the following options:

* **cjdctl start**: Run cjdroute and the daemon, and tell the daemon to start cjdroute at runtime and when the phone wakes (this state will continue after restarting.)
* **cjdctl stop**: Stop cjdroute and tell the daemon not to start cjdroute at runtime or when the phone wakes (this state will continue after restarting.)
* **cjdctl restart**: Run the stop and start commands above, in that order.
* **cjdctl start-daemon**: Start the daemon if it isn_t running, which will then start cjdroute on init and when the phone wakes based on the state set by the most recent start/stop command, and stop cjdroute when the phone sleeps.
* **cjdctl stop-daemon**: Stop the daemon if it's running, preventing cjdroute from being stopped when the phone sleeps and started when it wakes.
* **Note**: Once cjdns starts successfully after running `cjdctl start`, it will automatically start on boot and stop/start on sleep/wake until the `cjdctl stop` is run.
