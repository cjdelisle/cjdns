# cjdns

[Русская версия] (README_RU.md)
[Hrvatski] (README_HR.md)
[Svenska] (README_SV.md)
[Ελληνικά] (README_GR.md)
[English] (README.md)

#### *Networking wynalezione na nowo*

Cjdns realizuje zaszyfrowaną sieć IPv6 przy użyciu kryptografii klucza publicznego dla
przydzielanie adresów i rozproszona tablica mieszająca routingu. zapewnia to
networking niemal zerowej konfiguracji i zapobiega wielu z bezpieczeństwem i
problemy skalowalności, które są plagą istniejących sieciach.

[![Build Status](https://travis-ci.org/cjdelisle/cjdns.svg?branch=master)](https://travis-ci.org/cjdelisle/cjdns)
[![Napiwek za następny "commit"] (https://tip4commit.com/projects/941.svg)] (https://tip4commit.com/github/cjdelisle/cjdns)
[![irc](https://img.shields.io/badge/irc%20chat-%23cjdns-blue.svg)](https://kiwiirc.com/client/irc.efnet.org/?nick=visitor|?#cjdns)

## Referencje

    23:26 <@jercos> dobrze, cjdns jest teraz oficjalnie bardziej niezawodny niż otwarty Internet dla dotarcia do moich tańszych VPSów :|

    12:52 <Mariner>, więc nie wiem, czy to zostało zrobione wcześniej, a ja zakładam, że jestoczywiste, ale myślę, że to miłe. Obecnie na hypie z samolotu.

    00:36 <tester> strony człowieka trwa tak długo, aby załadować na I2P
    00:36 <tester> wartość prędkości i ponad anonimowości lada dzień

    <DuoNoxSol> to zwłaszcza bardziej niezawodny niż normalny internet

    09:46 <Kubuxu> Kocham bazę kodu cjdns

    <Whyrusleeping> mój internet jest o wiele lepiej.
    <Whyrusleeping> dzięki
    <Whyrusleeping> Jestem bardzo zdenerwowany i smutny, że jej lepiej
    <Whyrusleeping> ale również bardzo zadowolony

    21:01 <@grewalsat> to jest niesamowite. z moimi workpalce wyników Speedtest.net uzyskać prędkość około 6 + mb, iz moje cjdns-gate jako sieć VPN Dostaję jak 11-15mb prędkości pobierania w speedtest.net
    21:01 <@grewalsat>: P
    21:01 <@grewalsat> plus dostęp do wszystkiego! :D

    <Davidar> Tak, muszę przyznać, że rodzaj uniknąć hypeirc powodu takich rzeczach

## Społeczność

* [irc://irc.efnet.org/#cjdns] [IRC internetowy]
* [Hyperboria] [] największa sieć cjdns, w październiku 2015 jest w niej 2100 węzłów.
* [Projekt Meshnet] []
* [/r/darknetplan] []
* [#cjdns na Twitterze] []


## Dokumentacja

* [Cele projektu] (doc/projectGoals.md)
* [Cjdns Whitepaper] (doc/Whitepaper.md)
* [Cjdns na Wikipedii] []

Zaawansowana konfiguracja:

* [Skonfiguruj bramkę NAT dla sieci LAN] (doc/nat-gateway.md)
* [Install cjdns na openindiana] (doc/open-indiana.md)

Dziękuję za poświęcony czas i zainteresowanie,

Twórcy cjdns.

--------------------------------------------------------------------------------

## Jak zainstalować cjdns

Te instrukcje są dla dystrybucji opartych na Debianie Linux oraz OS X. powinny one być tyle przydatne do stosowania na innych dystrybucjach - po prostu nie oczekuj od nich pracować bez żadnych modyfikacji.

### 0. Instalacja zależności

Na obu platformach, instalowanie [node.js](http://nodejs.org/), choć korzystne,
nie jest to bezwzględnie konieczne. Jeśli node.js jest niedostępny lub jest przestarzałą wersją, zostanie on pobrany i zainstalowany w drzewie źródłowym.

#### Distro oparte na Debianie:

    sudo apt-get install nodejs git build-essential

#### Dystrybucja w oparciu o Fedora 22+:

    sudo dnf install install nodejs git
    sudo dnf install @development-tools

#### Distro oparte o RHEL (dodaje repo EPEL):

    sudo yum localinstall https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
    sudo yum install install nodejs git
    sudo yum install @development-tools

#### OS X:

Instalowanie z homebrew:

    brew install cjdns

#### OpenBSD:

Niestety, wersja na OpenBSD jest teraz nieco eksperymentalna.

    pkg_add git node gcc gmake bash

Wybierz wersję gcc-4.8.1p2 lub nowszą.

#### FreeBSD:

Wszystko, czego potrzebujesz jest dostępny prebuildowanych portach FreeBSD.

    pkg install gmake node

### 1. Pobierz cjdns z GitHub

Sklonuj repozytorium z GitHub i przejdź do katalogu ze źródłami:

    git clone https://github.com/cjdelisle/cjdns.git cjdns
    cd cjdns

### 2. Wykonaj build

    ./do

Jak pojawi się: `Build completed successfully, type ./cjdroute to begin setup.`, wtedy idź dalej:

-------------------------------------------------- ------------------------------

## Ustawiienia

Uruchom cjdroute bez opcji o pomoc:

    ./cjdroute

### 0. Upewnij się, że masz na rzeczy.

    cat /dev/net/tun

Jeśli mówi: `cat: /dev/net/tun: File descriptor in bad state` to dobrze!

Jeśli mówi: `cat: /dev/net/tun: No such file or directory`, utwórz go za pomocą:

    sudo mkdir -p /dev/net &&
    sudo mknod /dev/net/tun c 10 200 &&
    sudo chmod 0666 /dev/net/tun

Wtedy `cat /dev/net/tun` ponownie.

Jeśli mówi: `cat: /dev/net/tun: Permission denied` prawdopodobnie używasz VPS
oparty na platformie OpenVZ wirtualizacji. Zapytaj swojego dostawcę, aby włączyć
Urządzenie TUN / TAP - jest to standardowy protokół, więc powinni wiedzieć, co dokładnie potrzeba. Jeśli jesteś na OS X, nie martw się o tym kroku.


### 1. Utwórz nowy plik konfiguracyjny

    ./cjdroute --genconf >> cjdroute.conf

** Chroń plik conf! **

Przegrany conf oznacza utraty hasła i przyłącza i kto podłączone do ciebie nie będzie już w stanie się połączyć. Przejęta conf oznacza, że inni ludzie mogą podszywać się pod ciebie w sieci.

Aby wygenerować plik conf z uprawnieniami ustawiony tak, że tylko Twój użytkownik może czytać i pisać do niego:

    (Umask 077 && ./cjdroute --genconf > cjdroute.conf)


### 2. Znajdź przyjaciela

Aby dostać się do istniejącej sieci (np Hyperboria), trzeba połączyć
osoba, która już znajduje się w sieci. Jest to wymagane dla szeregu
powodów:

1. To pomaga zapobiec nadużyciom, bo źli ludzie będą mniej skłonni do nadużywania
   System po byli, w akcie ludzkiej dobroci, biorąc pod uwagę, że dostęp do
   system.
2. To nie jest przeznaczony do nakładki The Old internet, jest on przeznaczony do zastąpienia
   to. Każde połączenie zostanie w odpowiednim czasie jest zastąpiona przez drut, światłowodowego
   kablowego lub bezprzewodowego połączenia sieciowego.
3. W każdym przypadku sporu, nie będzie "łańcuch przyjaciół" powiązanie
   osoby zaangażowane więc nie będzie już podstawą zbliża się do
   rozkład.

Aby znaleźć przyjaciela, wyjść i dołączyć do naszej [społeczności] (#community). Też mam
spojrzenie na [Hyperboria Map][], aby znaleźć rówieśników w pobliżu ciebie.


### 3. Podłącz węzłami twojego przyjaciela

**Aby zainicjować połączenie wychodzące**

W pliku conf, widać:

``` javascript
// Nodes to connect to.
"connectTo":
{
    // Add connection credentials here to join the network
    // Ask somebody who is already connected.
}
`` `

Plik conf z wieloma węzłami przyjaciel, konfiguracji wychodzących, powinna wyglądać następująco:

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
`` `

Można dodać dowolną liczbę połączeń, jak chcesz do atrybutu  `connectTo`, po składni JSON.


**Aby umożliwić przyjaciela do zainicjowania połączenia przychodzące**

W pliku conf, widać:

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


Trzeba dać William Jevons (dokonująca połączenia przychodzące) 4 następujące elementy:

1. Twój IPv4 zewnętrzne
2. Port znaleźć w pliku conf tutaj:

    `// Bind do tego portu.
    "Bind": "0.0.0.0:yourportnumberishere", `

3. Ich unikatowe hasło, które komentarzem lub utworzone: `"password:" thisisauniquestring_002"`
4. Klucz publiczny: `"publicKey": "thisisauniqueKEY_001.k"`
5. Jego nazwa: "William Jevons"

Jego dane logowania będzie wyglądać następująco (z Twojego IPv4 i port):

```javascript
"1.2.3.4:56789": {
    "login": "William Jevons",
    "password": "thisisauniquestring_002",
    "publicKey": "thisIsJustForAnExampleDoNotUseThisInYourConfFile_1.k"
}
```

Należy pamiętać, że ty i twój przyjaciel * inicjowanie * a
Połączenie zarówno wychodzące (od ciebie -> Friend) lub przychodzące (od przyjaciela -> TY)
ale ruch przepływa w obie strony po ustanowieniu połączenia.

Zobacz [doc / configure.md] (doc / configure.md) więcej szczegółów na temat konfiguracji
w tym, jak to peer z innymi cjdns węzłów poprzez ethernet i wifi.


### 4. Zabezpiecz swój system - sprawdź usług słuchania

Gdy węzeł działa, jesteś teraz świeżo upieczony hosta IPv6. Twój operacyjnego
System może automatycznie zmienić konfigurację usług sieciowych, aby korzystać z tego nowego adresu.
Jeśli tak nie jest, co zamierza, należy sprawdzić, czy nie jesteś
oferując więcej usług potem zamierzeniu. ;)

Zobacz [doc / network-services.md] (doc / network-services.md) w celu uzyskania instrukcji.


### 5. Uruchom go!

    sudo ./cjdroute < cjdroute.conf

Jeśli chcesz mieć swoje logi zapisywane w pliku:

    sudo ./cjdroute < cjdroute.conf > cjdroute.log

Aby zatrzymać cjdns:

    sudo killall cjdroute

Jeśli występują problemy użyć `killall cjdroute` aby powrócić do normalności. Posługiwać się
`Cjdroute` pgrep lub` top` aby sprawdzić, czy działa.

**UWAGA!**

Zaczyna cjdns jako użytkownik root, dzięki czemu można go skonfigurować swój system
bez troski o uprawnieniach. Aby rozpocząć cjdns jako użytkownik inny niż root, patrz
[Doc/non-root-user.md](doc / non-root-user.md).


### 6. Bądź w IRC

Witamy w sieci! Teraz jesteś administratorem sieci. Tam są
obowiązków, które pochodzą z bycia administratorem sieci, które obejmują
jest dostępny w przypadku coś jest nie tak ze sprzętem. Powinieneś
pobyt na [IRC] (# społeczności), tak, że ludzie mogą cię dosięgnąć.


## Admin interfejs

Gdy cjdroute jest uruchomiony i działa, interfejs administracyjny będzie dostępny pod adresem
`udp://localhost:11234` (można to zmienić w` cjdroute.conf`
Plik konfiguracyjny). Zobacz [doc/admin-api.md](doc/admin-api.md) więcej
informacje na temat interfejsu administratora. Istnieje kilka narzędzi w `contrib/`
które współdziałają z nim.

Możesz uzyskać dostęp do API administratora z:

* The **Biblioteka Pythona**; patrz [tutaj] (contrib / python / README.md).
* The **Biblioteka Perl**, utrzymywane przez Mikey; patrz [tutaj](contrib/Perl/CJDNS/README).


[IRC Web]: http://chat.efnet.org/irc.cgi?chan=%23cjdns
[Hyperboria]: http://hyperboria.net
[Projekt Meshnet]: https://projectmeshnet.org
[/r/darknetplan]: http://www.reddit.com/r/darknetplan
[#cjdns na Twitterze]: https://twitter.com/hashtag/cjdns
[Hyperboria mapa]: http://www.fc00.org/
[Buildbots]: https://buildbot.meshwith.me/cjdns/waterfall

[Cjdns Wikipedia]: https://en.wikipedia.org/wiki/Cjdns
[Ukazuje tablica mieszająca]: https://en.wikipedia.org/wiki/Distributed_hash_table
[Beyond Pain]: https://lists.torproject.org/pipermail/tor-dev/2012-October/004063.html
[Kademlia]: https://en.wikipedia.org/wiki/Kademlia

[Tora]: https://www.torproject.org
[I2P]: http://www.i2p2.de
[Freenet]: https://freenetproject.org