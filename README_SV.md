# cjdns

[Engelska](README.md)
[Ryska](README_RU.md)
[Kroatiska](README_HR.md)

#### *Datornätverk - Ett nytt sätt*

Cjdns är ett krypterat IPv6-nätverk. Adresser i nätverket baseras på assymetrisk
kryptografi och paket dirigeras i nätverket genom att använda en s.k. distribuerad
hashtabell (DHT, eller Distributed Hash Table på engelska). Genom denna design
uppnås ett nätverk som knappt kräver någon konfiguration. Vi blir också av med
många skalnings- och säkerhetsproblem som andra nätverk har.

[![Build Status](https://travis-ci.org/cjdelisle/cjdns.svg?branch=master)](https://travis-ci.org/cjdelisle/cjdns)
[![tip for next commit](https://tip4commit.com/projects/941.svg)](https://tip4commit.com/github/cjdelisle/cjdns)
[![irc](https://img.shields.io/badge/irc%20chat-%23cjdns-blue.svg)](https://kiwiirc.com/client/irc.efnet.org/?nick=visitor|?#cjdns)

## Vitsord

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
    ​<whyrusleeping> thanks
    <whyrusleeping> i'm really upset and sad that its better
    <whyrusleeping> but also quite happy

## Community

* [irc://irc.efnet.org/#cjdns][IRC Web]
* [Hyperboria][], det största cjdns-nätverket, över 2100 noder i Oktober 2015.
* [Project Meshnet][]
* [/r/darknetplan][]
* [#cjdns på Twitter][]

## Dokumentation

* [Projectmål](doc/projectGoals.md)
* [Cjdns Whitepaper](doc/Whitepaper.md)
* [Cjdns på Wikipedia][]

Avancerad konfiguration:

* [Sätt upp en cjdns NAT gateway till ditt LAN (EN)](doc/nat-gateway.md)
* [Installera cjdns på OpenIndiana (EN)](doc/open-indiana.md)

Tack för visat intresse,

- Utvecklarna av Cjdns.

--------------------------------------------------------------------------------

## Hur du installerar cjdns

Följande instruktioner är till för Debian-baserade Linux-distributioner eller för OS X.
De bör vara tillräckligt informativa för att kunna anpassas till andra distributioner -
förvänta dig dock inte att de kommer fungera rakt av.

### 0. Installera beroenden

Cjdns byggsystem beror av [Node.js](http://nodejs.org/). Det är att föredra om du
kan installera node.js på din maskin, men det är inte nödvändigt. Om node.js inte finns
installerat eller om det är en oacceptabel version kommer node.js att laddas ner och
installeras i kodträdet.

#### Debian-distibutioner

    sudo apt-get install nodejs git build-essential python2.7

#### Fedora 22+-distributioner

    sudo dnf install install nodejs git
    sudo dnf install @development-tools

#### RHEL-distributioner

    sudo yum localinstall https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
    sudo yum install install nodejs git
    sudo yum install @development-tools

#### OS X:

Installera med homebrew:

    brew install cjdns

#### OpenBSD:

OpenBSD är tyvärr inte stabil just nu.

    pkg_add git node gcc gmake bash

Välj gcc version 4.8.1p2 eller senare.

#### FreeBSD:

Allt du behöver finns tillgängligt i FreeBSD' ports.

    pkg install gmake node

### 1. Hämta cjdns från GitHub

Clone'a repositoryt från GitHub och byt till katalogen cjdns:

    git clone https://github.com/cjdelisle/cjdns.git cjdns
    cd cjdns

### 2. Bygg

    ./do

Vänta tills du ser följande meddelande `Build completed successfully, type ./cjdroute to begin setup.`,
fortsätt sedan med följande:

--------------------------------------------------------------------------------

## Upplägg

Kör cjdroute utan argument för HJÄLP:

    ./cjdroute

### 0. Kontrollera att du har allting som behövs.

    LANG=C cat /dev/net/tun

Om det står: `cat: /dev/net/tun: File descriptor in bad state` Bra!

Om det står: `cat: /dev/net/tun: No such file or directory`, skapa den med följande kommandon:

    sudo mkdir -p /dev/net &&
    sudo mknod /dev/net/tun c 10 200 &&
    sudo chmod 0666 /dev/net/tun

Prova sedan `cat /dev/net/tun` igen.

Om det står: `cat: /dev/net/tun: Permission denied` så använder du troligen en VPS
baserad på OpenVZ's virtualiseringsplattform. Fråga din leverantör om de kan
tillåta/öppna upp TUN/TAP - denna terminologi är standard så de bör förstå vad du
ber dem om. Om du använder OS X kan du ignorera detta steg.


### 1. Skapa en ny konfigurationsfil

    ./cjdroute --genconf >> cjdroute.conf

**Skydda din konfigurationsfil!**

En borttappad konfigurationsfil innebär att du tappat bort både ditt lösenord
och dina uppkopplingar och alla som någonsin varit uppkopplade mot dig,
kan inte längre göra det. En komprometterad konfigurationsfil innebär att någon
annan kan agera som dig på nätverket.

För att generera en konfigurationfil med rättigheter där bara du kan läsa
och skriva till den:

    (umask 077 && ./cjdroute --genconf > cjdroute.conf)


### 2. Hitta en vän

För att koppla upp dig mot ett redan existerande nätverk (t ex Hyperboria)
måste du koppla upp dig till någon som redan tillhör nätverket. Detta är
nödvändigt av ett antal orsaker:

1. Det hjälper till att förhindra missbruk då det är osannolikt att
   dåliga människor kommer missbruka ett system de fått tillgång till
   från en annan människa som de på något sätt har en relation till.
2. Cjdns är inte tänkt att vara ett overlay-nätverk ovanpå internet,
   utan tanken är att det ska byta ut det. Varje uppkoppling kommer i sinom
   tid att bytas ut mot en sladd, en fiberoptisk kabel eller en wifi.
3. Vid händelse av meningsskiljaktigheter kommer det finnas en "kedja
   av vänner" som länkar ihop alla involverade så det finns redan en
   bas för att komma fram till en lösning.

För att hitta en vän, gå med i vår [community](#community). Kolla även in
[Hyperboria Map][] för att hitta peers nära dig.


### 3. Koppla din nod till din väns nod

**För att skapa en utgående uppkoppling**

Leta upp `connectTo`-attributet i din konfigurationsfil:

``` javascript
// Nodes to connect to.
"connectTo":
{
    // Add connection credentials here to join the network
    // Ask somebody who is already connected.
}
```

En konfigurationsfil med flera utgående vän-noder, kan se ut:

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

Du kan lägga till så många uppkopplingar du vill till `connectTo`-attributet.


**För att tillåta din vän att göra ingående uppkoppling**

Leta upp `authorizedPasswords`-attributet i din konfigurationsfil:

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

En konfigurationsfil med flera vän-noder uppsatta som ingående kan se ut:

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

Du behöver ge William Jevons (som kopplar upp sig mot dig) följande fyra saker:

1. Din externa IPv4-adress
2. Den port som finns i din konfigurationsfil här:

    `// Bind to this port.
    "bind": "0.0.0.0:yourportnumberishere",`

3. Deras unika lösenord som du kommenterade ut eller skapade: `"password": "thisisauniquestring_002"`
4. Din publika nyckel: `"publicKey": "thisisauniqueKEY_001.k"`

Hens loginuppgifter kommer då se ut som följer (med din IPv4-adress och port):

```javascript
"1.2.3.4:56789": {
    "login": "William Jevons",
    "password": "thisisauniquestring_002",
    "publicKey": "thisIsJustForAnExampleDoNotUseThisInYourConfFile_1.k"
}
```

Var vänlig notera att både du och din vän kan *påbörja* en uppkoppling
outbound (från DIG --> VÄN) eller inbound (från VÄN --> DIG) men trafiken
flödar i båda riktningarna så fort ni har kopplat upp er mot varandra.

Se [doc/configure.md](doc/configure.md) för mer information om konfigurering,
inkluderat hur du kan koppla upp dig mot andra cjdns noder över ethernet och wifi.


### 4. Säkra ditt system - kontrollera lyssnande tjänster

När din nod är uppe och kör är den en IPv6 host. Operativsystemet kan
automatiskt omkonfigurera nätverksservices att använda den nya adressen.
Om detta *inte* är vad du ville borde du kontrollera att du inte erbjuder
fler tjänster än vad du avsåg. ;)

Se [doc/network-services.md](doc/network-services.md) för instruktioner.


### 5. Starta din nod!

    sudo ./cjdroute < cjdroute.conf

Om du vill logga till fil:

    sudo ./cjdroute < cjdroute.conf > cjdroute.log

För att avsluta cjdns:

    sudo killall cjdroute

`sudo killall cjdns` är också användbart om du får problem. Använd
`pgrep cjdroute` eller `top` för att kolla om cjdroute kör eller ej.

*OBS!*

Detta kör cjdns som användaren root så det kan konfigurera
ditt system utan att behöva bry sig om rättigheter. För att starta cjdns utan
root-rättigheter, se [doc/non-root-user.md](doc/non-root-user.md).


### 6. Logga in på IRC

Välkommen till nätverket! Du är nu en nätverksadministrator och med det
kommer vissa skyldigheter, som bl.a. innebär att du bör vara tillgänglig
i händelse av att någonting inte stämmer med din utrustning. Enklast är
att logga in på [IRC](#community) så att andra kan nå dig.


## Admingränssnittet

När cjdroute är uppe och kör så finns det ett admingränssnitt tillgängligt
på `udp://localhost:11234` (detta kan ändras i `cjdroute.conf`).
Se [doc/admin-api.md](doc/admin-api.md) för mer information om admingränssnittet.

Det finns flera olika verktyg i `contrib/` som kan interagera med det, bl.a.:

* **python**; se [contrib/python/README.md](contrib/python/README.md).
* **perl**, maintained by Mikey; se [contrib/perl/CJDNS/README](contrib/perl/CJDNS/README).

[IRC Web]: http://chat.efnet.org/irc.cgi?chan=%23cjdns
[Hyperboria]: http://hyperboria.net
[Project Meshnet]: https://projectmeshnet.org
[/r/darknetplan]: http://www.reddit.com/r/darknetplan
[#cjdns på Twitter]: https://twitter.com/hashtag/cjdns
[Hyperboria Map]: http://www.fc00.org/
[Buildbots]: https://buildbot.meshwith.me/cjdns/waterfall

[Cjdns på Wikipedia]: https://en.wikipedia.org/wiki/Cjdns
[Distributed Hash Table]: https://en.wikipedia.org/wiki/Distributed_hash_table
[Beyond Pain]: https://lists.torproject.org/pipermail/tor-dev/2012-October/004063.html
[Kademlia]: https://en.wikipedia.org/wiki/Kademlia

[Tor]: https://www.torproject.org
[I2P]: http://www.i2p2.de
[Freenet]: https://freenetproject.org
