# CJDNS Android & Firefox OS #

## BUILD INSTRUCTIONS ##

These instructions have been tested with various Androids, and Firefox OS build 20140627162151.

### Requirements ###

* **Git**: Required to clone the git repo.
* **Node.js**: Required to run the build script.
* **Python 2**: Required to build a dependency during the build process.
* **Android NDK** (optional): Required to compile the project for Android (will be downloaded if not available.)

### Setup ###

* **Configure Location of Android NDK**: If you have a version of the Android NDK installed, and it isn't pointed to by the (standard) variable: `$ANDROID_NDK`, set the `$NDK` variable in your shell to point to the directory it's located. (eg: `export NDK="/opt/android-ndk"`)
* **Configure Alternative Repository**: To use an alternative repository, open `cjdroid-build.sh` and edit `REPO=https://github.com/cjdelisle/cjdns.git` to point to the URL of your desired repository. (eg: `REPO="http://newrepo"`)
  * **NOTE**: You must remove the `cjdns-android/cjdns` folder if it exists after changing the repo before you can run `cjdroid-build.sh` again.
* **Configure Alternative Branch**: To use a branch other than `master`, open `cjdroid-build.sh` and edit `BRANCH="master"` to use the name of your desired branch. (eg: `BRANCH="newbranch"`)

### Build ###

To clone a fresh repo and build, run `./cjdroid-build.sh` or `./cjdroid-build.sh branchname`.

## DEPLOYMENT INSTRUCTIONS ##

### Requirements ###

* **Root**: Root privileges are required to install things in the `/system` partition, and to configure the network using tun. The method required to root a phone differs from model to model.  If your phone isn't rooted yet and you're not sure where to start, look for the subforum for your device on [XDA forums](http://forum.xda-developers.com), and hopefully you'll find something that works.
* **Tun**: This provides the interface cjdns uses to create a virtual network device. Most (if not all) 4.0+ phones include tun support. If yours uses 2.x, CyanogenMod and some stock ROMs include support, but many don't. If your phone doesn't have a TUN device at /dev/tun, download [com.aed.tun.installer](http://cjdns.ca/com.aed.tun.installer.apk), then install and run it to have it set one up for you.

### Package ###

Below is a list of the files contained in the deployment package named:~ `cjdroid-${version}.tar.gz`:

* **install.sh** (run by **user** as **root**): This remounts `/system` read/write, installs the files described below to their respective locations, then remounts `/system` read-only.
* **files/99cjdroute**: Deployed to `/system/etc/init.d/99cjdroute`, this is run at boot to start `cjdaemon`.
* **files/cjdaemon**: Deployed to `/system/bin/cjdaemon`, this starts cjdroute if it's supposed to be running, then stops it when the phone sleeps and starts it again if it's supposed to be running when the phone wakes back up.
* **files/cjdctl**: Deployed to `/system/bin/cjdctl`, this lets you control cjdroute as well as the daemon.
* **files/cjdroute**: This is the `cjdroute` binary, which can be used like normal to generate a config file with: `cjdroute --genconf > /sdcard/cjdns/cjdroute.conf`, and you can run: `cjdroute < /sdcard/cjdns/cjdroute.conf` to start it manually (running it manually may result in higher than normal battery drain in sleep mode).
* **NOTE**: The files included in `cjdroid-${version}.tar.gz` are from the repo cloned by `cjdroid-build.sh` @  `cjdns-android/cjdns` (and not the current repository, if you're running a copy of `cjdroid-build.sh` from inside one.)

### Deploy ###

After `./cjdroid-build.sh` finishes building `./build_android/cjdns/cjdroute`, it will generate a deployment package named `cjdroid-${version}.tar.gz`. (eg: `cjdroid-0.3.2380.tar.gz`)

1. Copy the compressed file to `/sdcard/` on your Android device. (eg: `adb push cjdroid-${version}.tar.gz /sdcard/`)
2. Get a shell running on your Android device. (eg: `adb shell`)
3. Navigate to `/sdcard/` and extract the compressed file: `tar zxvf cjdroid-${version}.tar.gz`
4. Execute the install script as root: `su -c "sh cjdroid/install.sh"`
  * **NOTE**: This will need to be done again whenever the system partition is wiped.
5. Create and configure or copy a premade `cjdroute.conf` to `/sdcard/cjdns/cjdroute.conf`.

## OPERATION INSTRUCTIONS ##

After completing the **Setup Instructions**, cjdns can be controlled as root by running `cjdctl` with a valid option. (eg: su -c "cjdctl e")

### Options ###

* `cjdctl e` or `cjdctl enable` : Starting at boot, cjdroute will run whenever the phone is awake.
* `cjdctl d` or `cjdctl disable`: Kill cjdroute and prevent it from starting whenever the phone is awake.

### Issues ###

If you followed the instructions and things don't appear to be working, try some of these ideas to see if you can figure out what's wrong:

* **Test Debugging Output**: Starting `cjdroute` manually by running `cjdroute < /sdcard/cjdns/cjdroute.conf` should output to stdout, which you can use to help debug configuration issues.
* **Test Ping**: Find the ipv6 of one of your peers and run `ping6 ipv6address`
* **Test Admin Port**: Configure the **admin** section on your Android device's **cjdroute.conf** to bind to the ipv4 of your local network, then connect remotely from a PC using **admin port** tools from `cjdns/contrib/`.
* **Does Config Exist?**: A valid `cjdroute.conf` must exist @ `/sdcard/cjdns/cjdroute.conf` for `cjdctl` or `cjdaemon` to start `cjdroute`.
* **Are Process Running?**: Ensure cjdroute is running with the following command: `ps | grep cjdroute`
* **Is Network Configured?**: Check that your tun device (usually tun0) was configured correctly (exists and has an ipv6 starting with fc*) by running: `ip addr`
