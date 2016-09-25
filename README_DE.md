# cjdns

[README.md](README.md)

#### *Netzwerk neu erfunden*

Cjdns implementiert ein verschlüsseltes IPv6 Netzwerk basierend auf Public-Key Kryptografie für die Adressen-Zuteilung und es benutzt eine verteilte Hash-Tabelle für das Routing.
Dies ermöglicht eine nahezu konfigurationslose Handhabung und verhindert viele Sicherheits- und Skalierungs-Probleme, welche andere existierende Netzwerke heimsuchen.

[![Build Status](https://travis-ci.org/cjdelisle/cjdns.svg?branch=master)](https://travis-ci.org/cjdelisle/cjdns)
[![tip for next commit](https://tip4commit.com/projects/941.svg)](https://tip4commit.com/github/cjdelisle/cjdns)
[![irc](https://img.shields.io/badge/irc%20chat-%23cjdns-blue.svg)](https://kiwiirc.com/client/irc.efnet.org/?nick=visitor|?#cjdns)

## Testimonials

    23:26 <@jercos> well, cjdns is now officially more reliable than the open
                    internet for getting to my cheaper VPSes :|

    12:52 < mariner> so i don't know if it's been done before, and i assume it's
                     obvious, but I think it's neat. Currently on hype from an
                     airplane

    00:36 < tester> man sites take so long to load on i2p
    00:36 < tester> i value speed over anonymity any day

    <DuoNoxSol> it's notably more reliable than the normal internet

    09:46 < Kubuxu> I so love cjdns code base

    <whyrusleeping> my internet is way better now.
    <whyrusleeping> thanks
    <whyrusleeping> i'm really upset and sad that its better
    <whyrusleeping> but also quite happy

    21:01 <@grewalsat> this is amazing. with my workpalce speedtest.net results I get around 6+mb speed, and with my cjdns-gate as vpn network I'm getting like 11-15mb download speed in speedtest.net
    21:01 <@grewalsat> :P
    21:01 <@grewalsat> plus, access anything! :D

    <davidar> Yeah, I have to admit I sort of avoided hypeirc because of stuff like that

## Community

* [irc://irc.efnet.org/#cjdns][IRC Web]
* [Hyperboria][] das grösste cjdns-Netzwerk mit bis zu 2100 Knotenpunkten (Oktober 2015).
* [Project Meshnet][]
* [/r/darknetplan][]
* [#cjdns auf Twitter][]


## Dokumentation

* [Projekt-Ziele](doc/projectGoals.md)
* [Cjdns Whitepaper](doc/Whitepaper.md)
* [Cjdns auf Wikipedia][]

Erweiterte Konfiguration:

* [Installiere ein cjdns NAT gateway für dein LAN](doc/nat-gateway.md)
* [Installiere cjdns auf OpenIndiana](doc/open-indiana.md)

Danke für deine Zeit und dein Interesse,

Die cjdns-Entwickler.

--------------------------------------------------------------------------------

## Installation von cjdns

Diese Anleitungen sind für Debian-basierte Linux-Distributionen und OS X. Sie sollten erklärend genug sein, um sie auch auf anderen Distributionen zu verwenden - wenn auch nicht unbedingt eins-zu-ein.

### 0. Installations-Abhängigkeiten

Für beide Platformen ist es nicht zwingend notwendig, doch aber vorzuziehen, [Node.js](http://nodejs.org/) zu installieren. Wenn Node.js nicht verfügbar oder eine nicht akzeptierte Version ist, wird es in den Quellpfad heruntergeladen und installiert.

#### Debian-basierte distro:

    sudo apt-get install nodejs git build-essential

#### Fedora 22+ basierte distro:

    sudo dnf install install nodejs git
    sudo dnf install @development-tools

#### RHEL basierte distro (adds the EPEL repo):

    sudo yum localinstall https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
    sudo yum install install nodejs git
    sudo yum install @development-tools

#### OS X:

Installation mit homebrew:

    brew install cjdns

#### OpenBSD:

OpenBSD ist momentan leider ein bisschen experimentell.

    pkg_add git node gcc gmake bash

Wähle die Version gcc-4.8.1p2 oder eine jüngere.

#### FreeBSD:

Alles was du benötigst ist als Prebuild im FreeBSD' port bereits vorhanden.

    pkg install gmake node

### 1. Retrieve cjdns from GitHub

Klone das Repository von GitHub und wechsle zum Quellverzeichnis:

    git clone https://github.com/cjdelisle/cjdns.git cjdns
    cd cjdns

### 2. Build

    ./do

Es sollte folgendes stehen: `Build completed successfully, type ./cjdroute to begin setup.`. Danach gehe vor wie weiter unten beschriebe:

--------------------------------------------------------------------------------

## Setup

Lasse cjdroute laufen ohne Optionen für HELP:

    ./cjdroute

### 0. Stelle sicher, dass du alles bekommen hast.

    cat /dev/net/tun

Wenn steht: `cat: /dev/net/tun: File descriptor in bad state` Gut!

Wenn steht: `cat: /dev/net/tun: No such file or directory`, erstelle es wie folgt:

    sudo mkdir -p /dev/net &&
    sudo mknod /dev/net/tun c 10 200 &&
    sudo chmod 0666 /dev/net/tun

Danach wieder `cat /dev/net/tun`.

Wenn es sagt: `cat: /dev/net/tun: Permission denied` Dann verwendest du vielleicht einen VPS basierend auf der OpenVZ virtualisations Platform. Frage deinen Provider, ob er das TUN/TAP device - das ist standard Protokoll, sie sollten also genau wissen was du benötigst. Wenn du auf OS X bist, musst du dich nicht um dieses Setup kümmern.


### 1. Erstelle ein neues Konfiguration-File

    ./cjdroute --genconf >> cjdroute.conf

**Schütze dein conf File!**

Ein verlorenes conf File bedeutet, du hast dein Passwort und deine Verbindungen verloren und jeder der sich mit dir verbunden hatte wird nicht länger in der lage sein sich mit dir zu verbinden.
Ein kompromitiertes conf File bedeutet, dass andere Leute sich mit deiner Identität im Netzwerk ausgeben können.

Um ein conf File mit den richtigen Erlaubnissen zu generieren, so dass nur dein User darauf schreiben und lesen kann:

    (umask 077 && ./cjdroute --genconf > cjdroute.conf)


### 2. Finde einen Freund

Um in ein existierendes Netzwerk (z.B. Hyperboria) zu gelangen, musst du dich mit jemandem verbinden, der bereits mit diesem Netzwerk verbunden ist. Dies ist wegen verschiedensten Gründen notwendig:

1. Es hilft den Missbrauch zu verhindern, denn schlechte Leute werden tendenziell ein System weniger missbrauchen, wenn ihnen, in einem Akt von menschlicher Freundlichkeit, von anderen der Zugang gewährt wird.
2. Es ist nicht geplant Das Alte Internet zu überlagern, sondern es zu ersetzen. Jede Verbindung wird zu gegebener Zeit durch ein Kabel, ein Glasfaser-Kabel oder eine Wireless-Netzwerk-Verbindung ersetzt.
3. Im Falle einer Streitigkeit wird es eine "Kette von Freunden" geben, welche die involvierten Leute verbindet, so dass es bereits eine Basis zu einer Lösungsfindung gibt.

Um einen Freund zu finden, gehe nach draussen und beteilige dich an unserer [Community](#community). Weiter kannst du auch einen Blick auf die [Hyperboria-Karte][] werfen, um Peers in deiner Nähe zu finden.


### 3. Verbinde deinen Knoten mit dem deines Freundes

**Um ein AUSgehende Verbindung herzustellen**

In deinem conf File wirst du folgendes sehen:

``` javascript
// Nodes to connect to.
"connectTo":
{
    // Add connection credentials here to join the network
    // Ask somebody who is already connected.
}
```

Ein conf File mit mehreren konfigurierten Freundes-Knoten (Setup AUSgehende Verbindung) sollte wie folgt aussehen:

``` javascript
// Nodes to connect to.
"connectTo":
{
    //friend_1 (IPv4: 0.1.2.3; IPv6 fcaa:5bac:66e4:713:cb00:e446:c317:fc39)
    "0.1.2.3:45678":
    {
        "login": "k.alexander"
        "password": "thisIsNotARealConnection_1",
        "publicKey": "thisIsJustForAnExampleDoNotUseThisInYourConfFile_1.k"
    }

    //friend_2 (IPv4: 5.1.2.3; IPv6 fcbb:5bac:66e4:713:cb00:e446:c317:fc39)
    "5.1.2.3:5678":
    {
        "login": "k.alexander"
        "password": "thisIsNotARealConnection_2",
        "publicKey": "thisIsJustForAnExampleDoNotUseThisInYourConfFile_2.k"
    }
}
```

In der gegebenen JSON-Syntax kannst du so viele Verbindungen wie du willst zum `connectTo` Attribut hinzufügen.


**Um deinem Freund zu erlauben eine EINgehende Verbindung mit dir aufzubauen**

In deinem config File wirst du eine Passage wie folgt finden:

``` javascript
"authorizedPasswords":
[
    // A unique string which is known to the client and server.
    {"password": "thisisauniquestring_001"}

    // More passwords should look like this.
    // {"password": "thisisauniquestring_002"}
    // {"password": "thisisauniquestring_003"}
    // {"password": "thisisauniquestring_004"}
    ...

    // "your.external.ip.goes.here:45678":{"password": "thisisauniquestring_001","publicKey":thisisauniqueKEY_001.k"}

],
```

Ein config File mit mehreren Freundes-Knoten (Setup EINgehende Verbindung) sollte wie folgt aussehen:

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


Du musst William Jevons (welcher die EINgehende Verbindung bei sich einträgt) die folgenden 4 Objekte geben:

1. Deine externe IPv4-Adresse
2. Den Port, welchen du in deinem conf File wie folgt findest:

    `// Bind to this port.
    "bind": "0.0.0.0:yourportnumberishere",`

3. Das speziefische Passwort, welches du unkommentiert oder neu erstellt hast:
`"password": "thisisauniquestring_002"`
4. Deinen Public-Key: `"publicKey": "thisisauniqueKEY_001.k"`
5. Seinen Benutzernamen: "William Jevons"

Seine Zugangsdaten werde dann ungefähr wie folgt aussehen (mit deiner IPv4-Adresse und Port):

```javascript
"1.2.3.4:56789": {
    "login": "William Jevons",
    "password": "thisisauniquestring_002",
    "publicKey": "thisIsJustForAnExampleDoNotUseThisInYourConfFile_1.k"
}
```

Bitte beachte, dass du und dein Freund jeweils nur eine AUSgehende (von DIR -> Freund) oder EINgehende (von Freund -> DIR) Verbindung *eintragen* müsst. Der Datenverkehr kann dann aber doch in beide Richtungen fliessen, sobald die Verbindung steht.

Für mehr Details bezüglich Konfiguration, oder wie du mit anderen cjdns-Knoten über Ethernet und Wifi peerst, schaue dir das Dokument [doc/configure.md](doc/configure.md) an.


### 4. Sichere dein System ab - prüfe auf lauschende Services

Wenn dein Knoten einmal läuft, bist du ein neuwertiger IPv6 Host. Dein Betriebssystem wird wahrscheinlich automatisch deine Netzwerk-Services neu konfigurieren, um diese neue Adresse zu verwenden.
Wenn das nicht dein Ziel ist, solltest du überprüfen, dass du nicht mehr Services bereitstellst als du willst. ;)

Schaue dir [doc/network-services.md](doc/network-services.md) an für Anleitungen.


### 5. Starte es auf!

    sudo ./cjdroute < cjdroute.conf

Wenn du deine Logs in ein File geschrieben haben willst:

    sudo ./cjdroute < cjdroute.conf > cjdroute.log

Um cjdns zu stoppen:

    sudo killall cjdroute

Wenn du Probleme hast, benutze `killall cjdroute` um tabula rasa zu machen. Benutze
`pgrep cjdroute` oder `top` um zu überprüfen, ob cjdns läuft.

**Bemerkung!**

Die obigen Anleitungen starten cjdns als root-user. Es kann also dein System konfigurieren ohne dich um erlaubniss zu bitten. Um cjdns als non-root-user zu starten, konsultiere [doc/non-root-user.md](doc/non-root-user.md).


### 6. Komme zu IRC

Willkommen im Netzwerk! Du bist jetzt ein Netzwerk-Administrator. Damit verbunden ist eine gewisse Verantwortung, die unter anderem darin bestehen erreichbar zu sein im Falle eines Problems mit deinen Gerätschaften. Du solltest dich auf [IRC](#community) aufhalten, so dass leute dich erreichen können.


## Admin interface

Wenn cjdroute gestartet wurde und läuft, kannst du das admin-Interface unter `udp://localhost:11234` erreichen (kann im folgenden Konfigurations-File geändert werden: `cjdroute.conf`). Konsultiere [doc/admin-api.md](doc/admin-api.md) für mehr Informationen bezüglich dem admin-Interface. Es gibt mehrere Tools in `contrib/`,
welche damit interagieren können.

Du kannst das admin-API wie folgt erreichen:

* die **Python library**; konsultiere [here](contrib/python/README.md).
* die **Perl library**, unterhalten by Mikey; konsultiere [here](contrib/perl/CJDNS/README).


[IRC Web]: http://chat.efnet.org/irc.cgi?chan=%23cjdns
[Hyperboria]: http://hyperboria.net
[Project Meshnet]: https://projectmeshnet.org
[/r/darknetplan]: http://www.reddit.com/r/darknetplan
[#cjdns auf Twitter]: https://twitter.com/hashtag/cjdns
[Hyperboria-Karte]: http://www.fc00.org/
[Buildbots]: https://buildbot.meshwith.me/cjdns/waterfall

[Cjdns auf Wikipedia]: https://en.wikipedia.org/wiki/Cjdns
[Distributed Hash Table]: https://en.wikipedia.org/wiki/Distributed_hash_table
[Beyond Pain]: https://lists.torproject.org/pipermail/tor-dev/2012-October/004063.html
[Kademlia]: https://en.wikipedia.org/wiki/Kademlia

[Tor]: https://www.torproject.org
[I2P]: http://www.i2p2.de
[Freenet]: https://freenetproject.org
