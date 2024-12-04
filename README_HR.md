# cjdns

[English](README.md)
[Русская версия](README_RU.md)
[Svenska](README_SV.md)
[Ελληνικά](README_GR.md)
[Deutsch](README_DE.md)
[繁體中文](README_ZHT.md)
[简体中文](README_ZHCN.md)
[Español](README_ES.md)
[Français](README_FR.md)
[Português brasileiro](README_PT-BR.md)

#### *Umrežavanje iznova*

Cjdns je šifrirana IPv6 mreža koja koristi public-key kriptografiju za
dodjelu adresa i DHT za usmjeravanje. To omogućuje gotovo nikakvu
mrežnu konfiguraciju i sprječava mnoge sigurnosne i
skalabilne probleme koje muče trenutne mreže.

[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/387/badge)](https://bestpractices.coreinfrastructure.org/projects/387)
[![napojnica za sljedeću promjenu](https://tip4commit.com/projects/941.svg)](https://tip4commit.com/github/cjdelisle/cjdns)
[![irc](https://img.shields.io/badge/irc%20chat-%23cjdns-blue.svg)](https://kiwiirc.com/client/irc.efnet.org/?nick=visitor|?#cjdns)
![License](https://img.shields.io/github/license/cjdelisle/cjdns.svg)

## Iskustva

    23:26 <@jercos> well, cjdns is now officially more reliable than the open
                    internet for getting to my cheaper VPSes :|

    12:52 < mariner> so i don't know if it's been done before, and i assume it's
                     obvious, but I think it's neat. Currently on hype from an
                     airplane

    00:36 < tester> man sites take so long to load on i2p
    00:36 < tester> i value speed over anonymity any day

    <DuoNoxSol> it's notably more reliable than the normal internet
    <DuoNoxSol> even though it really really shouldn't be
    <DuoNoxSol> seeing as the connections are largely over the normal internet


## Zajednica

* [irc://irc.efnet.org/#cjdns][IRC Web]
* [Hyperboria][]
* [Projekt Meshnet][]
* [/r/darknetplan][]
* [#cjdns na Twitteru][]


## Dokumentacija

* [Ciljevi projekta](doc/projectGoals.md)
* [Cjdns Whitepaper](doc/Whitepaper.md)
* [Cjdns na Wikipediji][]


Napredna konfiguracija:

* [Setup a cjdns NAT gateway for your LAN](doc/nat-gateway.md)
* [Instaliraj cjdns na OpenIndiani](doc/open-indiana.md)

Hvala Vam za vaše vrijeme i interes,

Cjdns programeri.

--------------------------------------------------------------------------------

## Kako instalirati cjdns

Ove instrukcije su namijenjene za distribucije bazirane na Debian Linux-u i macOS-u.
Trebale bi biti dovoljno informativne za korištenje na ostalim distribucijama,
ali ne očekujte da će raditi od prve.

### 0. Install dependencies

On both platforms, installing [Node.js](https://nodejs.org/), although preferable,
is not strictly necessary. If Node.js is unavailable or an unacceptable version,
it will be downloaded and installed in the source tree.

#### Distribucija bazirana na Debianu:

    sudo apt-get install nodejs git build-essential python2.7

#### Distribucija bazirana na Fedori 22+:

    sudo dnf install install nodejs git
    sudo dnf install @development-tools
    
#### Distribucija bazirana na RHELu (dodaje EPEL repozitorij):

    sudo yum localinstall https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
    sudo yum install install nodejs git
    sudo yum install @development-tools

#### macOS:

Instalacija putem [Homebrewa](https://brew.sh/):

    brew install cjdns

Instalacija putem [MacPortsa](https://www.macports.org/):

    sudo port install cjdns

#### OpenBSD:

    pkg_add git node gcc gmake bash

Odaberite verziju gcc-4.8.1p2 ili noviju.

#### FreeBSD:

    pkg install gmake node
    
#### Arch:

Možete instalirati cjdns sljedećom naredbom;

    pacman -S cjdns

If you need to build from source, everything you need can be installed like this

    pacman -S nodejs git base-devel

Alternatively, you may like to install via AUR from the package, `cjdns-git`.
Nakon instalacije konfiguracijska datoteka će se nalaziti u `/etc/cjdroute.conf`.
Za pokrenuti servis `cjdns.service`, pokrenite sljedeću naredbu:

        systemctl start cjdns

Za zaustavljanje:

       systemctl stop cjdns

#### Gentoo:

cjdns is not yet in the main Gentoo repository, so you will have to use an overlay.
The easiest way is to use Layman but you can do it by hand, too.

##### Layman:

Prvo treba te instalirati layman:

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
    
### 1. Preuzmite cjdns sa GitHuba

Clone the repository from GitHub and change to the source directory:

    git clone https://github.com/cjdelisle/cjdns.git cjdns
    cd cjdns

### 2. Build

    ./do

Potražite `Build completed successfully, type ./cjdroute to begin setup.`, zatim nastavite:

--------------------------------------------------------------------------------

## Setup

Run cjdroute without options for HELP:

    ./cjdroute

### 0. Provjerite imate li potrebne stvari

    LANG=C cat /dev/net/tun

Ako kaže: `cat: /dev/net/tun: File descriptor in bad state`, sve je u redu!

Ako kaže: `cat: /dev/net/tun: No such file or directory`, stvorite ga putem:

    sudo mkdir -p /dev/net &&
    sudo mknod /dev/net/tun c 10 200 &&
    sudo chmod 0666 /dev/net/tun

Zatim opet pokrenite `cat /dev/net/tun`.

Ako kaže: `cat: /dev/net/tun: Permission denied` vjerojatno koristite VPS
baziran na OpenVZ virtualizacijskoj platformi. Pitajte vašeg pružatelja usluga da vam omogući
TUN/TAP - ovo je standardni protokol tako da bi trebali znati što trebate.
Ako ste na macOSu, ne morate se brinuti u vezi ove greške.


### 1. Stvorite novu konfiguracijsku datoteku

    ./cjdroute --genconf >> cjdroute.conf

**Zaštitite Vašu konfiguraciju!** A lost conf file means you lost your password and
connections and anyone who connected to you will no longer be able to connect.
A compromised conf file means that other people can impersonate you on the
network.

To set generate a conf file with permissions set so that only your user can
read it and write to it:

    (umask 077 && ./cjdroute --genconf > cjdroute.conf)


### 2. Pronađite prijatelja

Kako biste ušli u postojeću mrežu (npr. Hyperboria), morate se spojiti s nekime
tko je već unutar te mreže. To je tako zbog nekoliko razloga:

1. It helps prevent abuse because bad people will be less likely to abuse a
   system after they were, in an act of human kindness, given access to that
   system.
2. This is not intended to overlay The Old Internet, it is intended to replace
   it. Each connection will in due time be replaced by a wire, a fiber optic
   cable, or a wireless network connection.
3. In any case of a disagreement, there will be a "chain of friends" linking
   the people involved so there will already be a basis for coming to a
   resolution.

To find a friend, get out there and join our [community](#community). Also, have
a look at the [Hyperboria Map][] to find peers near you.


### 3. Spojite vaš node sa nodeom prijatelja

**Za pokretanje izlazne konekcije**

U Vašoj conf datoteci vidjeti ćete:

``` javascript
// Nodes to connect to.
"connectTo":
{
    // Add connection credentials here to join the network
    // Ask somebody who is already connected.
}
```

A conf file with multiple friend-nodes, setup OUTbound, should look like:

``` javascript
// Nodes to connect to.
"connectTo":
{
    //friend_1 (IPv4: 0.1.2.3; IPv6 fcaa:5bac:66e4:713:cb00:e446:c317:fc39)
    "0.1.2.3:45678":
    {
        "password": "thisIsNotARealConnection_1",
        "publicKey": "thisIsJustForAnExampleDoNotUseThisInYourConfFile_1.k"
    }

    //friend_2 (IPv4: 5.1.2.3; IPv6 fcbb:5bac:66e4:713:cb00:e446:c317:fc39)
    "5.1.2.3:5678":
    {
        "password": "thisIsNotARealConnection_2",
        "publicKey": "thisIsJustForAnExampleDoNotUseThisInYourConfFile_2.k"
    }
}
```

You can add as many connections as you want to the `connectTo` attribute,
following JSON syntax.


**To allow your friend to initiate the connection INbound**

U Vašoj conf datoteci vidjeti ćete:
``` javascript
"authorizedPasswords":
[
    // A unique string which is known to the client and server.
    {"password": "password001", "login": "default-login"}

    // More passwords should look like this.
    // {"password": "password002", "login": "my-second-peer"}
    // {"password": "password003", "login": "my-third-peer}
    // {"password": "password004", "login": "my-fourth-peer"}
    ...

    // "your.external.ip.goes.here:45678":{"login": "default-login", "password": "password001","publicKey":thisisauniqueKEY_001.k"}

],
```

A conf file with multiple friend-nodes, setup INbound, should look like:
``` javascript
"authorizedPasswords":
[
    // A unique string which is known to the client and server.
    {"password": "thisisauniquestring_001", "user": "k.alexander"}

    // More passwords should look like this.
    //William Jevons (IPv4: 0.1.2.3; IPv6 fcaa:5bac:66e4:713:cb00:e446:c317:fc39)
    {"password": "thisisauniquestring_002", "user": "William Jevons"}
    //Marilyn Patel (IPv4: 5.1.2.3; IPv6 fcbb:5bac:66e4:713:cb00:e446:c317:fc39)
    {"password": "thisisauniquestring_003", "user": "Marilyn Patel"}
    // {"password": "thisisauniquestring_004"}
    ...

    // "your.external.ip.goes.here:45678":{"password": "thisisauniquestring_001","publicKey":thisisauniqueKEY_001.k"}
],
```


Morate dati friend_3 (who is making the INbound connection) sljedeće 4 stvari:

1. Vašu vanjsku IPv4 adresu
2. Port iz konfiguracijske datoteke:

                `// Bind to this port.
                "bind": "0.0.0.0:yourportnumberishere",`

3. Their unique password that you uncommented or created: `"password": "thisisauniquestring_002"`
4. Vaš javni ključ: `"publicKey": "thisisauniqueKEY_001.k"`



Please note that you and your friend can *initiate* a
connection either outbound (from YOU --> FRIEND) or inbound (from FRIEND --> YOU)
but traffic flows both ways once the connection is established.


See [doc/configure.md](doc/configure.md) for more details on configuration,
including how to peer with other cjdns nodes over ethernet and wifi.


### 4. Zaštitite Vaš sustav - check for listening services

Nakon što se Vaš node pokrene, postajete IPv6 host. Vaš operativni
sustav će automatski konfigurirati mrežne servise kako bi koristili novu adresu.
Ako ovo nije Vaš cilj, trebali bi ste provjeriti da ne pružate više servisa
nego što mislite. ;)

Pogledajte [doc/network-services.md](doc/network-services.md) za instrukcije.


### 5. Pokrenite ga!

    sudo ./cjdroute < cjdroute.conf

Ako želite da se logovi zapisuju u datoteku:

    sudo ./cjdroute < cjdroute.conf > cjdroute.log

Za zaustavljanje cjdns-a:

    sudo killall cjdroute

Ako imate problema koristite `killall cjdroute`.
Koristite `pgrep cjdroute` ili `top` kako bi ste provjerili radi li cjdns.

**Note:** ovo pokreće cjdns kao putem korisnika tako da može konfigurirati Vaš sustav
bez potrebnih dozvola. Za pokretanje cjdns-a putem običnog korisnika, pogledajte
[doc/non-root-user.md](doc/non-root-user.md).


### 6. Dođite na IRC

Dobrodošli u mrežu! Sada ste mrežni administrator. Kao mrežni
administrator odgovorni ste za stvari poput kvara opreme.
Budite na [IRC-u](#community) kako bi vas ljudi mogli kontaktirati.


## Administratorsko sučelje

Kada je cjdnroute pokrenut, administratorsko sučelje biti će dostupna na
`udp://localhost:11234` (ovo se može promijeniti u cjdroute.conf
konfiguracijskoj datoteci). Pogledajte [doc/admin-api.md](doc/admin-api.md) za više
informacija o administratorskom sučelju. U `contrib/` se nalazi nekoliko alata
s kojima može komunicirati.

Možete pristupiti administratorskom API-ju putem:

* the **Python library**; pogledajte
  [contrib/python/README.md](contrib/python/README.md).
* the **Perl library**, održava Mikey; pogledajte
  [contrib/perl/CJDNS/README](contrib/perl/CJDNS/README).


[IRC Web]: http://chat.efnet.org/irc.cgi?chan=%23cjdns
[Hyperboria]: https://hyperboria.net
[/r/darknetplan]: https://www.reddit.com/r/darknetplan
[#cjdns na Twitteru]: https://twitter.com/hashtag/cjdns
[Hyperboria Map]: https://www.fc00.org/
[Buildbots]: https://buildbot.meshwith.me/cjdns/waterfall

[Cjdns na Wikepediji]: https://en.wikipedia.org/wiki/Cjdns
[Distribuirana hash tablica]: https://en.wikipedia.org/wiki/Distributed_hash_table
[Beyond Pain]: https://lists.torproject.org/pipermail/tor-dev/2012-October/004063.html
[Kademlia]: https://en.wikipedia.org/wiki/Kademlia

[Tor]: https://www.torproject.org
[I2P]: https://geti2p.net/en/
[Freenet]: https://freenetproject.org
