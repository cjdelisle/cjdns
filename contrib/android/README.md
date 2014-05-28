# CJDNS Android #

## BUILD CJDNS for Android ##

### Dependencies ###

* **Git**: Required to clone the git repo.
* **Node.js**: Required to run the build script.
* **Python 2**: Required to build a dependency during the build process.
* **Android NDK** (optional): Required to compile the project for Android (will be downloaded if not available.)

### Configure Build ###

* **Configure Location of Android NDK**: If you have a version of the Android NDK installed, and it isn't pointed to by the (standard) variable: _$ANDROID_NDK_, set the _$NDK_ variable in your shell to point to the directory it's located. (eg: `export NDK="/opt/android-ndk"`)
* **Configure Alternative Repository**: To use an alternative repository, open _cjdroid-build.sh_ and edit _REPO=https://github.com/cjdelisle/cjdns.git_ to point to the URL of your desired repository. (eg: _REPO="http://newrepo"_)
  * **NOTE**: You must remove the _cjdns-android/cjdns_ folder if it exists after changing the repo before you can run _cjdroid-build.sh_ again.
* **Configure Alternative Branch**: To use a branch other than _master_, open _cjdroid-build.sh_ and edit _BRANCH="master"_ to use the name of your desired branch. (eg: _BRANCH="newbranch"_)

### Build and Package ###

Run _cjdroid-build.sh_ and wait until it completes.

## Deploy CJDNS for Android ##

### Requirements ###

* **Root**: Root privileges are required to install things in the _/system_ partition, and to configure the network using tun. The method required to root a phone differs from model to model.  If your phone isn't rooted yet and you're not sure where to start, look for the subforum for your device on [XDA forums](http://forum.xda-developers.com), and hopefully you'll find something that works.
* **Tun**: This provides the interface cjdns uses to create a virtual network device. Most (if not all) 4.0+ phones include tun support. If yours uses 2.x, CyanogenMod and some stock ROMs include support, but many don't. If your phone doesn't have a TUN device at /dev/tun, download [com.aed.tun.installer](http://cjdns.ca/com.aed.tun.installer.apk), then install and run it to have it set one up for you.

### Setup Instructions ###

After _cjdroid-build.sh_ finishes building _cjdroute_, it will generate a compressed file in the current directory named _cjdroid-${version}.tar.gz_ with the current commit in place of ${version} (eg: _cjdroid-0.3.2380.tar.gz_)

1. Copy the compressed file to your Android device using a method you're comfortable with, such as running: `adb push cjdroid-${version}.tar.gz /sdcard/`
2. Get a shell running on your Android device:
  * From your PC using adb: `adb shell`
  * On your Android device using _Terminal Emulator_.
3. Extract the compressed file: `tar zxvf /sdcard/cjdroid-${version}.tar.gz`
4. Become root by running: `su`
5. Execute the install script by running: `sh /sdcard/cjdroid/install.sh`
  * **NOTE**: Wiping the _/system_ partition for any reason, including to update your ROM, will remove the installed files and disable cjdns until this command is run again.
6. Create a directory in _/sdcard_ named _cjdns_ by running: `mkdir /sdcard/cjdns`
7. Either copy a premade cjdroute.conf file to _/sdcard/cjdns/cjdroute.conf_, or create a new one by running: `cjdroute --genconf > /sdcard/cjdns/cjdroute.conf`
8. Add peers to the connectTo section of _/sdcard/cjdns/cjdroute.conf_ if necessary.
9. Run `cjdctl start` to start _cjdaemon_ and _cjdroute_.

### Commands ###

Once _install.sh_ is run and _/sdcard/cjdns/cjdroute.conf_ exists, _cjdctl_ can be run as root from the command line with the following options:

* cjdctl **start**: Run cjdroute and the daemon + set the daemon to start _cjdroute_ on boot and wakeup.
* cjdctl **stop**: Stop cjdroute + set the daemon to do nothing on boot and wakeup.
* cjdctl **restart**: Run the stop command above followed by the start command above.
* cjdctl **start-daemon**: Start the daemon if it isn't running (does not start _cjdroute_ unless the daemon has been set to do so.)
* cjdctl **stop-daemon**: Stop the daemon if it's running (does not stop _cjdroute_.)
* **NOTE**: Remember that once `cjdctl start` has been run, cjdroute will start on boot and wakeup until running the `cjdctl stop` command.

### Issues ###

If you followed the instructions and things don't appear to be working, try some of these ideas to see if you can figure out what's wrong:

* **Test Debugging Output**: Starting _cjdroute_ manually by running `cjdroute < /sdcard/cjdns/cjdroute.conf` should output to stdout, which you can use to help debug configuration issues.
* **Test Ping**: Find the ipv6 of one of your peers and run `ping6 ipv6address`
* **Test Admin Port**: Configure the _admin_ section on your Android device's _cjdroute.conf_ to bind to the ipv4 of your local network, then connect remotely from a PC and using tools from _contrib_.
* **Does Config Exist?**: A valid _cjdroute.conf_ must exist @ _/sdcard/cjdns/cjdroute.conf_ for _cjdctl_ or _cjdaemon_ to start _cjdroute_.
* **Are Process Running?**: Ensure cjdroute is running with the following command: `ps | grep cjdroute`
* **Is Network Configured?**: Check that your tun device (usually tun0) was configured correctly (exists and has an ipv6 starting with fc*) by running: `ip addr`

### Files ###

Below is a list of the files contained in _cjdroid-${version}.tar.gz_:

* **install.sh** (run by _user_ as _root_): This remounts _/system_ read/write, installs the files described below to their respective locations, then remounts _/system_ read-only.
* **files/99cjdroute** (run by _system_): Deployed to _/system/etc/init.d/99cjdroute_, this is run at boot to start _cjdaemon_.
* **files/cjdaemon** (run by _system_ and _cjdctl_): Deployed to _/system/bin/cjdaemon_, this starts cjdroute if it's supposed to be running, then stops it when the phone sleeps and starts it again if it's supposed to be running when the phone wakes back up.
* **files/cjdctl** (run by _user_ as _root_): Deployed to _/system/bin/cjdctl_, this lets you control cjdroute as well as the daemon.
* **files/cjdroute** (run by _user_ as _root_ and _system_): This is the _cjdroute_ binary, which can be used like normal to generate a config file with: `cjdroute --genconf > /sdcard/cjdns/cjdroute.conf`, and you can run: `cjdroute < /sdcard/cjdns/cjdroute.conf` to start it manually (running it manually may result in higher than normal battery drain in sleep mode).
* **NOTE**: The files included in _cjdroid-${version}.tar.gz_ are from the repo cloned by _cjdroid-build.sh_ @  _cjdns-android/cjdns_ (and not the current repository, if you're running a copy of _cjdroid-build.sh_ from inside one.)
