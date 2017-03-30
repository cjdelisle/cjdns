Traduction à partir de la version
[`cjdns-v19.1`](https://github.com/woshilapin/cjdns/blob/cjdns-v19.1/README.md)
# cjdns

[Русская версия](README_RU.md)
[Hrvatski](README_HR.md)
[Svenska](README_SV.md)
[Ελληνικά](README_GR.md)
[Deutsch](README_DE.md)
[繁體中文](README_ZHT.md)
[Español](README_ES.md)
[Français](README_FR.md)

#### *Le réseau réinventé*

Cjdns est un réseau IPv6 chiffré utilisant des clefs publiques de cryptographie
pour l'allocation d'adresses et une table de hashasge distribuée pour le
routage.  Il fournit un réseau quasiment sans configuration et évite la majorité
des problèmes de sécurité et de passage à l'échelle (_scalability_) des réseaux
actuels.

[![Statut de compilation](https://api.travis-ci.org/cjdelisle/cjdns.svg?branch=master)](https://travis-ci.org/cjdelisle/cjdns)
[![Bonnes pratiques CII](https://bestpractices.coreinfrastructure.org/projects/387/badge)](https://bestpractices.coreinfrastructure.org/projects/387)
[![Pourboire pour le prochain commit](https://tip4commit.com/projects/941.svg)](https://tip4commit.com/github/cjdelisle/cjdns)
[![IRC](https://img.shields.io/badge/irc%20chat-%23cjdns-blue.svg)](https://kiwiirc.com/client/irc.efnet.org/?nick=visitor|?#cjdns)
![Licence](https://img.shields.io/github/license/cjdelisle/cjdns.svg)

## Témoignages

    23:26 <@jercos> well, cjdns is now officially more reliable than the open
                    internet for getting to my cheaper VPSes :|

    23:26 <@jercos> bon, cjdns est maintenant officiellement plus fiable que
                    l'internet pour me connecter à mes VPS bon marchés :|


    12:52 < mariner> so i don't know if it's been done before, and i assume it's
                     obvious, but I think it's neat. Currently on hype from an
                     airplane

    12:52 < mariner> je ne sais pas si ça a déjà été fait avant, et je suppose
                     que c'est facile, mais je pense que c'est génial.  En ce
                     moment sur Hype depuis un avion


    00:36 < tester> man sites take so long to load on i2p
    00:36 < tester> i value speed over anonymity any day

    00:36 < tester> les sites prennent tellement longtemps à charger sur I2P
    00:36 < tester> je préfèrerai toujours la vitesse face à l'anonymité


    <DuoNoxSol> it's notably more reliable than the normal internet

    <DuoNoxSol> c'est particulièrement plus fiable que l'internet normal


    09:46 < Kubuxu> I so love cjdns code base

    09:46 < Kubuxu> J'adore tellement le code source de cjdns


    <whyrusleeping> my internet is way better now.
    <whyrusleeping> thanks
    <whyrusleeping> i'm really upset and sad that its better
    <whyrusleeping> but also quite happy

    <whyrusleeping> mon internet est vachement mieux maintenant.
    <whyrusleeping> merci
    <whyrusleeping> je suis vraiment énervé et triste que ce soit mieux
    <whyrusleeping> mais aussi pas mal content


    21:01 <@grewalsat> this is amazing. with my workpalce speedtest.net results I get around 6+mb speed, and with my cjdns-gate as vpn network I'm getting like 11-15mb download speed in speedtest.net
    21:01 <@grewalsat> :P
    21:01 <@grewalsat> plus, access anything! :D

    21:01 <@grewalsat> c'est fou. avec les scores speedtest.net depuis mon
                       boulot, j'ai un débit d'environ 6+mb, et avec une
                       passerelle cjdns comme réseau VPN, j'obtiens autour de
                       11-15mb en téléchargement sur speedtest.net
    21:01 <@grewalsat> :P
    21:01 <@grewalsat> en plus, accès à tout ! :D


    <davidar> Yeah, I have to admit I sort of avoided hypeirc because of stuff like that

    <davidar> Ouais, je dois admettre que j'ai en quelque sorte évité hypeirc à
              cause de ce genre de truc

## Community

* [irc://irc.efnet.org/#cjdns][IRC Web]
* [Hyperboria][] le plus grand réseau cjdns, avec 2100 noeuds en Octobre 2015
* [Projet Meshnet][]
* [/r/darknetplan][]
* [#cjdns sur Twitter][]


## Documentation

* [Objectifs du project](doc/projectGoals.md)
* [Livre blanc sur cjdns](doc/Whitepaper.md)
* [Cjdns sur Wikipedia][]

Configuration avancée:

* [Configurer une passerelle NAT cjdns pour votre LAN](doc/nat-gateway.md)
* [Installer cjdns sur OpenIndiana](doc/open-indiana.md)

Merci pour votre temps et votre intérêt,

Les développeurs de cjdns.

--------------------------------------------------------------------------------

## Comment insaller cjdns

Ces instructions sont pour les distributions Linux basées sur Debian et OS X.
Elles devraient être suffisantes pour d'autres distributions mais n'espérez
qu'elles fonctionnent mot pour mot.  Si vous souhaitez connaitre la base de
votre système d'exploitation, [regardez le graphique suivant](https://upload.wikimedia.org/wikipedia/commons/1/1b/Linux_Distribution_Timeline.svg).

### 0. Installer les dépendances

Installer [Node.js](https://nodejs.org/), bien que préférable,
n'est pas nécessaire.  Si Node.js n'est pas disponible ou que la version n'est
pas la bonne, Node.js sera téléchargé et installé dans le répertoire du code
source.

#### Distributions basées sur Debian :

    sudo apt-get install nodejs git build-essential python2.7

#### Distributions basées sur Fedora 22 ou + :

    sudo dnf install install nodejs git
    sudo dnf install @development-tools

#### Distributions basées sur RHEL (ajoutez le dépôt EPEL) :

    sudo yum localinstall https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
    sudo yum install install nodejs git
    sudo yum install @development-tools

Compiler depuis le paquetage :
`sudo yum localinstall https://kojipkgs.fedoraproject.org//packages/cjdns/17.4/4.el6/src/cjdns-17.4-4.el6.src.rpm`

Si vous êtes sur un ordinateur portable et que vous le mettez en veille ou en
hibernation, cjdroute prendra du temps (faites vous un café !) pour comprendre
ce qui vient de se passer en tentant de se réveiller.  Vous pouvez radicalement
améliorer la vitesse de ce réveil avec :

    systemctl enable cjdns-resume

Le service de réveil redémarre cjdns lorsque le système sort de veille ou
d'hibernation.

#### Gentoo

    emerge --ask nodejs sys-devel/gcc dev-lang/python:3.4 dev-vcs/git

#### OS X :

Installer avec homebrew :

    brew install cjdns

#### OpenBSD :

Malheureusement, le support sur OpenBSD en encore un peu expérimental.

    pkg_add git node gcc gmake bash

Utilisez gcc-4.8.1p2 ou plus récent.

#### FreeBSD :

Tout ce dont vous avez besoin est disponible pré-compilé dans les ports FreeBSD.

    pkg install gmake node

#### Arch :

Vous pouvez installer cjdns en lançant

    pacman -S cjdns

Si vous avez besoin de compiler à partir des sources, tout ce dont vous avez
besoin peut être installer avec la commande suivante

    pacman -S nodejs git base-devel

Sinon, vous préférez peut-être installer à travers AUR à partir du paquetage
`cjdns-git`.  Après l'installation, vous trouverez le fichier de configuration
dans `/etc/cjdroute.conf`.  Pour démarrer le service `cjdns.service`, faites :

        systemctl start cjdns

Pour l'arrêter, faites :

       systemctl stop cjdns

#### Gentoo :

cjdns n'est pas encore dans le dépôt principal de Gentoo, donc vous allez devoir
utiliser un arbre alternatif (_overlay_).  La manière la plus simple est
d'utiliser Layman mais vous pouvez également le faire à la main.

##### Layman :

Premièrement, vous devez installer layman.

      emerge layman

Si layman est installé correctement, vous pouvez ajouter un arbre alternatif

      layman -f
      layman -a weuxel

Pour des mises à jour futures de l'arbre alternatif, utilisez

      layman -S

Maintenant, vous pouvez installer cjdns

      emerge cjdns

##### À la main :

Vous allez devoir cloner l'arbre alternatif depuis le dépôt

       cd /opt
       git clone https://github.com/Weuxel/portage-weuxel.git

Maintenant, configurez portage pour utiliser ce dépôt

       cd /etc/portage/repos.conf/

Créez un fichier `portage-weuxel.conf` contenant

       [weuxel]
       location = /opt/portage-weuxel
       masters = gentoo
       auto-sync = yes

Maintenant, synchronisez

       emerge --sync

Puis installez cjdns

   emerge cjdns

#### Détection automatique de plantage et redémarrage

Copiez le script init openrc `contrib/openrc` vers `/etc/init.d/` et modifiez
les paramètres `CONFFILE` et `command` selon vos besoins.  Puis démarrez cjdns

   /etc/init.d/cjdns start

Configurez le système init pour démarrer automatiquement cjdns

   rc-update add cjdns default

Copiez le script `contrib/gentoo/service_restart.sh` vers un répertoire
approprié sur votre système et modifiez l'adresse de courriel.  Si vous ne
souhaitez pas recevoir de notifications, commentez la ligne.  Maintenant,
ajoutez une ligne dans votre crontab de la manière suivante

   # Redémarrer le service planté
   * * * * *       root	/path/to/script/service_restart.sh

#### Solus :

Dépendances:

      sudo eopkg install nodejs git build-essential system.devel python gcc binutils kernal-headers xorg-server-devel

Puis suivez les étapes suivantes :

*Désolé pour le nombre important d'étapes.  Un paquetage est en cours de
préparation*

##### 1. Récupérez cjdns sur Github

Clonez le dépôt depuis Github et déplacez vous dans le nouveau répertoire :

    git clone https://github.com/cjdelisle/cjdns.git cjdns
    cd cjdns

##### 2. Compilez

    ./do

Cherchez pour la ligne `Build completed successfully, type ./cjdroute to begin
setup.`, puis continuez ci-dessous :

--------------------------------------------------------------------------------

## Configuration

Lancer cjdroute sans options pour obtenir de l'aide :

    ./cjdroute

### 0. Vérifiez que vous avez tout.

Si vous êtes sur OS X, ne vous inquiétez pas pour cette étape.

    LANG=C cat /dev/net/tun

Si le message est : `cat: /dev/net/tun: File descriptor in bad state` Parfait !

Si le message est : `cat: /dev/net/tun: No such file or directory`, créez le avec :

    sudo mkdir -p /dev/net &&
    sudo mknod /dev/net/tun c 10 200 &&
    sudo chmod 0666 /dev/net/tun

Puis `cat /dev/net/tun` à nouveau.

Si le message est : `cat: /dev/net/tun: Permission denied`, vous utilisez probablement un VPS tournant sur une platform virtualisée avec OpenVZ.  Demandez à votre fournisseur d'activer le périphérique TUN/TAP - c'est un protocole standard, ils devraient savoir exactement ce dont vous avez besoin.


### 1. Générez un nouveau fichier de configuration

    ./cjdroute --genconf >> cjdroute.conf

**Protégez votre fichier de configuration !**

Un fichier de configuration perdu signifie que vous avez perdu votre mot de
passe et les connexions; toutes les personnes connectées à vous n'en serons plus
capables.  Un fichier de configuration compromis signifie que d'autres personnes
peuvent se faire passer pour vous sur le réseau.

Pour générer un fichier de configuration avec des droits tels que votre
utilisateur puisse lire et écrire dedans :

    (umask 077 && ./cjdroute --genconf > cjdroute.conf)


### 2. Trouvez des amis

Pour entrer dans un réseau existant (par exemple, Hyperboria), vous devez vous
connecter à quelqu'un qui est déjà dans le réseau.  C'est nécessaire pour les
raisons suivantes :

1. Cela évite les abus car les personnes nuisibles seront moins prompts à abuser
   le système si l'accès leur a été offert dans un acte de bonté
2. L'intention n'est pas de faire une surcouche au Vieil Internet, l'objectif
   est de le remplacer.  Chaque connexion sera à terme, remplacée par un cable,
   une fibre optique ou une connexion sans fil.
3. Dans le cas d'un désaccord, une « chaine d'amis » liant les personnes
   impliquées fournira une base afin d'aboutir à une solution.

Pour trouver un ami, sortez dehors and rejoignez notre [communité](#community).  Vous pouvez également jetez un oeil à la 
[carte Hyperboria][] pour trouvez des pairs près de chez vous.


### 3. Connectez votre noeud à celui de votre ami

**Pour démarrer une connexion OUTbound**

Dans votre fichier de configuration, vous trouverez :

``` javascript
// Nodes to connect to.
"connectTo":
{
    // Add connection credentials here to join the network
    // Ask somebody who is already connected.
}
```

Un fichier de configuration avec plusieurs noeuds amis OUTbound devrait
ressembler à ceci :

``` javascript
// Nodes to connect to.
"connectTo":
{
    //ami_1 (IPv4: 0.1.2.3; IPv6 fcaa:5bac:66e4:713:cb00:e446:c317:fc39)
    "0.1.2.3:45678":
    {
        "login": "k.alexander"
        "password": "thisIsNotARealConnection_1",
        "publicKey": "thisIsJustForAnExampleDoNotUseThisInYourConfFile_1.k"
    },

    //ami_2 (IPv4: 5.1.2.3; IPv6 fcbb:5bac:66e4:713:cb00:e446:c317:fc39)
    "5.1.2.3:5678":
    {
        "login": "k.alexander"
        "password": "thisIsNotARealConnection_2",
        "publicKey": "thisIsJustForAnExampleDoNotUseThisInYourConfFile_2.k"
    }
}
```

Vous pouvez ajouter autant de connexions que vous le voulez dans la propriété
`connectTo`, en respectant la syntaxe JSON.


**Pour permettre à votre ami de démarrer une connexion INbound**

Dans votre fichier de configuration, vous trouverez :

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

Un fichier de configuration avec plusieurs noeuds amis INbound, devrais
ressembler à ceci :
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


Vous devez donner à William Jevons (qui se connectera en INbound) les 4 éléments suivants :

1. Votre adresse IPv4 externe
2. Le port que vous trouverez dans votre fichier de configuration à la ligne

    `// Bind to this port.
    "bind": "0.0.0.0:yourportnumberishere",`

3. Un mot de passe unique que vous décommentez ou que vous créez : `"password": "thisisauniquestring_002"`
4. Votre clef publique : `"publicKey": "thisisauniqueKEY_001.k"`
5. Son nom d'utilisateur : "William Jevons"

Ces paramètres de connexion devraient ressembler à quelque chose comme ceci
(avec votre adresse IPv4 et votre port) :

```javascript
"1.2.3.4:56789": {
    "login": "William Jevons",
    "password": "thisisauniquestring_002",
    "publicKey": "thisIsJustForAnExampleDoNotUseThisInYourConfFile_1.k"
}
```

Veuillez noter que vous et votre ami peuvent *démarrer* une connexion soit
OUTbound (de VOUS --> AMI) soit INbound (de AMI --> vous) mais la circulation se
fait dans les deux sens une fois que la connection est établie.

Voir [doc/configure.md](doc/configure.md) pour plus de détails sur la
configuration, en particulier sur la manière de se connecter à d'autres noeuds
cjdns à travers ethernet et wifi.


### 4. Sécurisez votre système - vérifiez les services ouverts

Une fois que votre noeud tourne, vous êtes maintenant le nouveau propriétaire
d'une adresse IPv6.  Votre système d'exploitation reconfigurera peut-être
automatiquement les services de réseau pour utiliser cette nouvelle adresse.  Si
ce n'est pas ce que vous souhaitez, vous devriez vérifier que vous n'offrez pas
plus de services que vous ne souhaitez vraiment. ;)

Voir [doc/network-services.md](doc/network-services.md) pour les instructions.


### 5. Démarrez-le !

    sudo ./cjdroute < cjdroute.conf

Si vous voulez écrire les messages du programme dans un fichier :

    sudo ./cjdroute < cjdroute.conf > cjdroute.log

Pour arrêter cjdns :

    sudo killall cjdroute

Si vous avez des problèmes, utilisez `kilall cjdroute` pour retrouver un état
propre.  Utilisez `pgrep cjdroute` ou `top` pour vérifier s'il est lancé.

**NOTE !**

Cette commande lance cjdns en tant qu'utilisateur root afin de pouvoir
configurer votre système sans soucis de droits.  Pour démarrer cjdns en tant
qu'utilisateur normal, voir [doc/non-root-user.md](doc/non-root-user.md).


### 6. Venir sur IRC

Bienvenue sur le réseau !  Vous êtes maintenant un administrateur réseau.
Certaines responsabilités viennent avec le fait d'être un administrateur réseau
incluant être disponible en cas de problème avec votre matériel.  Vous devriez
rester sur [IRC](#community) afin que les gens puissent vous joindre.


## Interface administrateur

Lorsque cjdroute est lancé, l'interface administrateur est disponible sur
`udp://localhost:11234` (ceci peut être modifié dans le fichier de configuration
`cjdroute.conf`). Voir [doc/admin-api.md](doc/admin-api.md) pour plus
d'informations à propos de l'interface administrateur.  Plusieurs outils pour
interagir avec cette interface sont disponibles dans `contrib/`.

Vous pouvez accéder à l'interface administrateur avec :

* la **bibliothèque Python**; voir [ici](contrib/python/README.md).
* la **bibliothèque Perl**, maintenue par Mikey; voir [ici](contrib/perl/CJDNS/README).


## Remonter un bogue
1. Ne remontez pas les bogues dans ce dépôt, veuillez les remonter au niveau de
   https://github.com/hyperboria/bugs/issues
2. Se connecter sur IRC et discuter avec quelqu'un
3. Qu'adviendra-t-il si
 * Quelqu'un se sent capable de le corriger
 * Vous vous sentez capable de le corriger
 * Personne ne se sent concerné et il sera oublié pendant une période jusqu'à ce
   que quelqu'un d'autre tombe dessus plus tard et le corrige ou il sera corrigé
   indirectement par une réorganisation du code
 * Personne ne peut le corriger pour le moment mais il semble nécessaire de s'en
   rappeler car il a un impact significatif sur la manière dont le projet est
   développé, dans ce cas, il est nécessaire qu'il soit expliqué en termes
   techniques par quelqu'un ayant une bonne connaissance du code du projet.  Ils
   devraient faire une requête au code (_pull request_) aux répertoires
   docs/bugs.

### Securité
Les bogues de sécurité devraient être remontés sur IRC de la même manière que
les autres bogues.  Nous n'avons pas de groupe de personne priviligiée avec une
connaissance spécifique, donc notre politique de remontée de bogues de sécurité
est une divulgation totale.
voir : https://github.com/cjdelisle/cjdns/blob/master/doc/security_specification.md pour voir si un bogue possible de sécurité en est vraiment un.

À ce moment de l'année, à nouveau... Il est temps pour quelques projets libres !
[![Hacktoberfest](https://img.shields.io/badge/Open%20Source-Hacktoberfest-orange.svg)](https://hacktoberfest.digitalocean.com/)

[IRC Web]: http://chat.efnet.org/irc.cgi?chan=%23cjdns
[Hyperboria]: https://hyperboria.net
[/r/darknetplan]: https://www.reddit.com/r/darknetplan
[#cjdns on Twitter]: https://twitter.com/hashtag/cjdns
[Hyperboria Map]: https://www.fc00.org/
[Buildbots]: https://buildbot.meshwith.me/cjdns/waterfall

[Cjdns on Wikipedia]: https://en.wikipedia.org/wiki/Cjdns
[Distributed Hash Table]: https://en.wikipedia.org/wiki/Distributed_hash_table
[Beyond Pain]: https://lists.torproject.org/pipermail/tor-dev/2012-October/004063.html
[Kademlia]: https://en.wikipedia.org/wiki/Kademlia

[Tor]: https://www.torproject.org
[I2P]: https://geti2p.net/en/
[Freenet]: https://freenetproject.org
