# Old Readme

The following is stuff from the readme file which is untested in a long time and therefore likely not working anymore.

Use at your own risk.

#### RHEL based distro (adds the EPEL repo):

    sudo yum localinstall https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
    sudo yum install nodejs git
    sudo yum install @development-tools
   
#### CentOS Stream 9
    sudo dnf install nodejs git
    sudo dnf groupinstall "Development Tools"
    curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh

#### Gentoo

    emerge --ask nodejs sys-devel/gcc dev-lang/python:3.4 dev-vcs/git

#### OpenBSD:

    pkg_add git node gcc gmake bash

Select version gcc-4.8.1p2 or more recent.

#### FreeBSD:

Everything you need is available prebuild in FreeBSD' ports.

    pkg install gmake node

#### Arch:

You can install cjdns by running:

    pacman -S cjdns

If you need to build from source, everything you need can be installed like this:

    pacman -S nodejs git base-devel

Alternatively, you may like to install via AUR from the package, `cjdns-git`.
After installation, the configuration file is located at `/etc/cjdroute.conf`.
To start the service `cjdns.service`, do:

    systemctl start cjdns

To stop it:

    systemctl stop cjdns

#### Gentoo:

cjdns is not yet in the main Gentoo repository, so you will have to use an overlay.
The easiest way is to use Layman but you can do it by hand, too.

##### Layman:

First, you need to install layman.

    emerge layman

If layman is installed correctly, you can add the overlay

    layman -f
    layman -a weuxel

For future update of the overlay use

    layman -S

Now you can install cjdns

    emerge cjdns

##### By hand:

You will have to clone the overlay repository

    cd /opt
    git clone https://github.com/Weuxel/portage-weuxel.git

Now tell portage to use this repo

    cd /etc/portage/repos.conf/

Create a file `portage-weuxel.conf` containing

    [weuxel]
    location = /opt/portage-weuxel
    masters = gentoo
    auto-sync = yes

Now sync

    emerge --sync

And install cjdns

    emerge cjdns

#### Automatic crash detection and restart

Copy the the openrc init script from `contrib/openrc` to `/etc/init.d/` and modify the `CONFFILE` and `command` parameter to your needs.
Then start cjdns by issuing

    /etc/init.d/cjdns start

Configure the init system to autostart cjdns

    rc-update add cjdns default

Copy the service_restart script `contrib/gentoo/service_restart.sh` to any convenient directory on
your system and modify the eMail address. If you do not wish to be notified, comment out the whole line.
Now add a crontab entry like this

    # Restart crashed Services
    * * * * *       root	/path/to/script/service_restart.sh

#### Solus:

Dependencies:

    sudo eopkg install nodejs git build-essential system.devel python gcc binutils kernal-headers xorg-server-devel

Then Follow the steps below:

*Sorry for so many steps. A package is being worked on currently*
