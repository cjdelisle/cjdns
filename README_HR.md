# cjdns

[English](README.md)
[Русская версия](README_RU.md)
[Svenska](README_SV.md)
[Ελληνικά](README_GR.md)
[Deutsch](README_DE.md)
[繁體中文](README_ZHT.md)
[Français](README_FR.md)

#### *Umrežavanje iznova*

Cjdns je šifrirana IPv6 mreža koja koristi public-key kriptografiju za
dodjelu adresa i distribuiranu hash tablicu za usmjeravanje. To omogućuje
gotovo nikakvu mrežnu konfiguraciju i sprječava mnoge sigurnosne i
skalabilne probleme koje muče trenutne mreže.

[![Build Status](https://api.travis-ci.org/cjdelisle/cjdns.svg?branch=master)](https://travis-ci.org/cjdelisle/cjdns)
[![napojnica za sljedeću promjenu](https://tip4commit.com/projects/941.svg)](https://tip4commit.com/github/cjdelisle/cjdns)

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


## Kada će biti završen?

[Hyperboria][] je najveća cjdns mreža, sa stotinama aktivnih jedinica
po cijelom svijetu.

Cjdns je testiran na x86, amd64, ARMv5, ARMv7, MIPS, and PowerPC.
Stalno se [testira][buildbots] na Linuxu, FreeBSDu, macOS-u, Windowsu and Illumos
systemima.

Protokoli i algoritmi su eksperimentalni i mogli bi se promijeniti.
Kako biste smanjili štetu mreže, molimo često nadograđujte vaše cjdns jedinke.


### I Vi možete pomoći!

We are in need of some buildbots on more obscure systems and architectures.
If you would like to donate one, you could mail it, or you could administer
it and provide remote shell access. Please email `buildbot@seattlemesh.net`
if you'd like to run a buildbot. Note that it is not a general support inbox,
other questions should be directed toward IRC.



## Kako radi usmjeravanje?

U cjdns mreži, paket ode do routera i router označi paket
sa instrukcijama do rutera koji će ga najbolje obratiti. That is, a router which
is physically nearby and has an address numerically close to the destination
address of the packet. The directions are added to the packet to allow it to go
through a number of routers with minimal handling, *a verifiable form of source
routing*. They just read the label and bounce the packet wherever the next bits
in the label tell them to. Routers have a responsibility to "keep in touch"
with other routers that are physically close by and numerically near to their
address.

Jezgra routera je modificirana implementacija [Kademlia][] distribuirane hash tablice.


## Zajednica

* irc://irc.efnet.org/#cjdns ([web client][IRC Web])
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

### Licenca

[Dostupna ovdje](LICENSE)

Hvala Vam za Vaše vrijeme i interes,

Cjdns programeri.

--------------------------------------------------------------------------------

## Kako instalirati cjdns

Ove instrukcije su namijenjene za distribucije bazirane na Debian Linux-u i OSX-u. Trebale bi biti
dovoljno informativne za korištenje na ostalim distribucijama, ali ne očekujte
da će raditi od prve.

### 0. Install dependencies

On both platforms, installing [Node.js](https://nodejs.org/), although preferable,
is not strictly necessary. If Node.js is unavailable or an unacceptable version,
it will be downloaded and installed in the source tree.

#### Distribucija bazirana na Debian-u:

    sudo apt-get install nodejs git build-essential python2.7

#### Distribucija bazirana na Fedori 22+:

    sudo dnf install install nodejs git
    sudo dnf install @development-tools
    
#### Distribucija bazirana na RHEL-u (dodaje EPEL repozitorij):

    sudo yum localinstall https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
    sudo yum install install nodejs git
    sudo yum install @development-tools

#### macOS:

On macOS, you must install the Command Line Developer Tools. If
you already have a recent version of Xcode (>= macOS 10.9 and >= Xcode 5.0.1), run the
following command:

    xcode-select --install

If Xcode is not installed, you can either install it through the App
Store and run the command above, or make a free Apple Developer account here:
[https://developer.apple.com/downloads/index.action](https://developer.apple.com/downloads/index.action).
Then sign in, search for Command Line Tools, and install the latest package
compatible with your version of macOS. If you encounter issues, there is a
thorough [stackoverflow post](https://stackoverflow.com/a/9329325) on installing
the Command Line Tools.

Morate instalirati Git i Node.js. Postoji nekoliko načina. Ako koristite [Homebrew](http://brew.sh/):

    brew install git nodejs

Ako koristite [MacPorts](https://www.macports.org/):

    sudo port install git nodejs6

Or if you use neither and would like to install the binaries from their websites:
doc
- Node.js: [https://nodejs.org/download/](https://nodejs.org/download/)
- git: [https://git-scm.com/download](https://git-scm.com/download)

#### OpenBSD:

Nažalost, OpenBSD je trenutno eksperimentalan.

    pkg_add git node gcc gmake bash

Odaberite verziju gcc-4.8.1p2 ili noviju.

#### FreeBSD:

The compiler expects GCC version 4.7, please install it from ports first.

    portsnap fetch extract
    cd /usr/ports/lang/gcc47/ && make config && make install clean

### 1. Retrieve cjdns from GitHub

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

Ako kaže: `cat: /dev/net/tun: File descriptor in bad state` Good!

Ako kaže: `cat: /dev/net/tun: No such file or directory`, create it using:

    sudo mkdir -p /dev/net &&
    sudo mknod /dev/net/tun c 10 200 &&
    sudo chmod 0666 /dev/net/tun

Zatim opet pokrenite `cat /dev/net/tun`.

Ako kaže: `cat: /dev/net/tun: Permission denied` You're probably using a VPS
based on the OpenVZ virtualization platform. Ask your provider to enable the
TUN/TAP device - this is standard protocol so they should know exactly what you
need. If you're on macOS, don't worry about this step.


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


### 3. Connect your node to your friend's node

**To initiate the connection OUTbound**

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
