# cjdns

[Русская версия](README_RU.md)
[Hrvatski](README_HR.md)
[Svenska](README_SV.md)
[Ελληνικά](README_GR.md)
[Deutsch](README_DE.md)
[繁體中文](README_ZHT.md)
[Español](README_ES.md)

#### *Reinventando las redes*

Cjdns implementa una red IPv6 cifrada usando criptografia de llave publica para
la asignación de direcciones y una tabla distribuida de hashes para el
ruteo. Esto provee redes de casi-nula-configuración, y previene mucho de los
problemas de seguridad y escalabilidad que plagan a las redes existentes.

[![Build Status](https://travis-ci.org/cjdelisle/cjdns.svg?branch=master)](https://travis-ci.org/cjdelisle/cjdns)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/387/badge)](https://bestpractices.coreinfrastructure.org/projects/387)
[![tip for next commit](https://tip4commit.com/projects/941.svg)](https://tip4commit.com/github/cjdelisle/cjdns)
[![irc](https://img.shields.io/badge/irc%20chat-%23cjdns-blue.svg)](https://kiwiirc.com/client/irc.efnet.org/?nick=visitor|?#cjdns)
![License](https://img.shields.io/github/license/cjdelisle/cjdns.svg)

## Testimonios

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

## Comunidad

* [irc://irc.efnet.org/#cjdns][IRC Web]
* [Hyperboria][] la mayor red cjdns, tomando a Octubre 2015 donde tenia 2100 nodos.
* [Project Meshnet][]
* [/r/darknetplan][]
* [#cjdns on Twitter][]


## Documentación

* [Metas del proyecto - en inglés -](doc/projectGoals.md)
* [Cjdns Whitepaper - en inglés -](doc/Whitepaper.md)
* [Cjdns on Wikipedia][]

## Configuración avanzada:

* [Configurando una puerta de enlace NAT en cjdns para tu LAN - en ingles](doc/nat-gateway.md)
* [Instalar cjdns en OpenIndiana - en ingles](doc/open-indiana.md)

Graciars por su tiempo e interes,

Los desarrolladores de CJDNS.

--------------------------------------------------------------------------------

## Como instalar CJDNS

Estas instrucciones son para las distribuciones basadas en Debian y macOS. Deben ser
suficientemente informativas como para usarse en otras distribuciones - solo no
espere que funcionen tal cual. Si desea saber que [base del sistema operativo tiene, revise aqui](https://upload.wikimedia.org/wikipedia/commons/1/1b/Linux_Distribution_Timeline.svg).

### 0. Instalar dependencias

En ambas plataformas, instalar [Node.js](http://nodejs.org/), aunque preferible,
no es estrictamente necesario. Si Node.js no esta disponible o en una version
inaceptable, sera descargado e instalado en el arbol de codigo fuente.

#### Distribuciones basadas en Debian:

    sudo apt-get install nodejs git build-essential python2.7

#### Distribuciones basadas en Fedora 22+:

    sudo dnf install install nodejs git
    sudo dnf install @development-tools

#### Distribuciones basada en RHEL (añade el repositorio EPEL):

    sudo yum localinstall https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
    sudo yum install install nodejs git
    sudo yum install @development-tools
Construyendo desde el paquete:
`sudo yum localinstall https://kojipkgs.fedoraproject.org//packages/cjdns/17.4/4.el6/src/cjdns-17.4-4.el6.src.rpm`

Si usted esta en una laptop y suspende o hiberna, a cjdroute le tomara unos
minutos figurar que ha pasado al despertar. Puede dramáticamente acelerar esto
con:

    systemctl enable cjdns-resume

El servicio de resume reinicia cjdns cuando el sistema regresa de dormir.

#### Gentoo

    emerge --ask nodejs sys-devel/gcc dev-lang/python:3.4 dev-vcs/git

#### macOS:

Instalar con [Homebrew](https://brew.sh/):

    brew install cjdns

Instalar con [MacPorts](https://www.macports.org/):

    sudo port install cjdns

#### OpenBSD:

Desafortunadamente, en OpenBSD es un poco experimental por el momento.

    pkg_add git node gcc gmake bash

Elija la version gcc-4.8.1p2 o mas reciente.

#### FreeBSD:

Todo lo que necesita esta prearmado en los ports de FreeBSD.

    pkg install gmake node

#### Arch:

Usted puede instalar cjdns ejecutando

    pacman -S cjdns

Si necesita armar desde el codigo fuente, todo lo requerido se puede obtener asi

    pacman -S nodejs git base-devel

Como alternativa, usted podria instalar desde AUR desde el paquete, `cjdns-git`.
Despues de instalar, La configuracion estara en `/etc/cjdroute.conf`.
Para iniciar el servicio `cjdns.service`, ejecute:

       systemctl start cjdns

Para detenerlo:

       systemctl stop cjdns

#### Solus:

Dependencias:

      sudo eopkg install nodejs git build-essential system.devel python gcc binutils kernel-headers xorg-server-devel

Despues siga estas instrucciones:

*Lo sentimos por tantos pasos. Actualmente un paquete se esta aun preparando*

### 1. Obtener cjdns desde GitHub

Clonar el repositorio desde GitHub y cambiar al directorio del codigo fuente:

    git clone https://github.com/cjdelisle/cjdns.git cjdns
    cd cjdns

### 2. Armar

    ./do

Espere por `Build completed successfully, type ./cjdroute to begin setup.`, y entonces 
proceda con lo que sigue abajo:

--------------------------------------------------------------------------------

## Configuración

Ejecute cjdroute sin opciones para otener ayuda:

    ./cjdroute

### 0. Cerciórese que tiene las cosas necesarias.

    LANG=C cat /dev/net/tun

Si dice: `cat: /dev/net/tun: File descriptor in bad state` ¡Muy bien!

Si dice: `cat: /dev/net/tun: No such file or directory`, tiene que crearlo ejecutando:

    sudo mkdir -p /dev/net &&
    sudo mknod /dev/net/tun c 10 200 &&
    sudo chmod 0666 /dev/net/tun

Después ejecute `cat /dev/net/tun` otra ves.

Si dice: `cat: /dev/net/tun: Permission denied` Probablemente usted esta usando
un VPS basado en la plataforma de virtualización OpenVZ. Pídale a su proveedor 
que habilite el dispositivo TUN/TAP - esto es un protocolo estándar y ellos
deberían de saber que es lo que usted necesita. Si esta usando macOS, no necesita
preocuparse por este paso.

### 1. Generando un archivo de configuración

    ./cjdroute --genconf >> cjdroute.conf

**¡Proteja su archivo conf!**

Un archivo conf extraviado significa perder sus contraseñas y conecciones
, y todo aquel que conectaba a usted ya no lo podrá hacer.
Un archivo conf que ha sido sustraido podría dar a que alguien se dedique
a impersonificarle.

Para generar un archivo conf con los permisos de tal manera que solo su
usuario pueda leer y escribir en el:

    (umask 077 && ./cjdroute --genconf > cjdroute.conf)


### 2. Encontrando un amigo

Para acceder a una red existente (por ejemplo, Hyperboria), necesita conectar con
alguien que ya tenga acceso a esa red. Esto es necesario por muchos motivos:

1. Ayuda a prevenir abusos porque la gente malintencionada muy probablemente evitara
   abusar un sistema después de que, en un acto de amabilidad, se le ha dado acceso
   al sistema.
2. Esto no intenta ser una capa sobre la Vieja Internet, su intención es remplazarla. 
   Cada conexión eventualmente debera ser remplazada por un cable, un enlace de fibra
   optica, o una conexión inalámbrica.
3. En cualquier caso de desacuerdo, existirá una "cadena de amigos" enlazando a la
   gente involucrada de manera de que exitiría una base para llegar a una resolución.

Para encontrar un amigo, salga de aquí y accede a nuestra [comunidad](#comunidad). También, 
de un vistazo a la [Hyperboria Map][] para encontrar nodos cercanos a usted.

### 3. Enlazar su nodo al nodo de su amigo

**Para iniciar una conexión OUTbound**

En su archivo conf, usted observará:

``` javascript
// Nodes to connect to.
"connectTo":
{
    // Add connection credentials here to join the network
    // Ask somebody who is already connected.
}
```

Un archivo conf con múltiples nodos de amigos, con la configuracion OUTbound, deberia
de verse así:

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

Usted puede agregar tantas conexiones como quiera en el atributo `connectTo`,
siguiendo la sintaxis de JSON.


**Para permitir que su amigo realize una conexión INbound**

En su archivo conf, usted encontrará:

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

Un archivo conf con múltiple nodos de amigos, con la configuración INbound, deberia de verse como:
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


Usted necesitaría darle a William Jevons (quien esta haciendo una conexion INbound) las siguiente 4 cosas:

1. Su IPv4 externa
2. El puerto que aquí se lee en su archivo conf:

    `// Bind to this port.
    "bind": "0.0.0.0:yourportnumberishere",`

3. El password único que ha descomentado o creado: `"password": "thisisauniquestring_002"`
4. Su llave pública: `"publicKey": "thisisauniqueKEY_001.k"`
5. Su nombre de usuario: "William Jevons"

Sus credenciales para iniciar sesion se veran como aquí (con su IPv4 y el puerto):

```javascript
"1.2.3.4:56789": {
    "login": "William Jevons",
    "password": "thisisauniquestring_002",
    "publicKey": "thisIsJustForAnExampleDoNotUseThisInYourConfFile_1.k"
}
```

De favor cerciorese que usted y su amigo pueden *iniciar* una
conexión tanto hacia fuera (de USTED --> AMIGO) hacia dentro (del AMIGO --> USTED)
pero el tráfico fluye ambos sentidos una vez una coneccion suceda.

Revise [doc/configure_ES.md](doc/configure_ES.md) para mas detalles de 
configuración, incluido como acceder a otros nodos cjdns sobre ethernet y wifi.


### 4. Asegurando su sistema - revisar los servicios que escuchan.

Una vez que su nodo esté corriendo, está ahora en una recién creada dirección IPv6. 
Su sistema operativo puede que configure automáticamente para que servicios sobre
red lo utilicen.
Si esta no es su intencion, deberá revisar que no se este ofreciendo mas servicios
que los deseados. ;)

Lea [doc/network-services.md](doc/network-services.md) - en inglés - para mas
instrucciones.


### 5. ¡Iniciarlo!

    sudo ./cjdroute < cjdroute.conf

Si desea que las bitácoras se escriban en un archivo:

    sudo ./cjdroute < cjdroute.conf > cjdroute.log

Para detener cjdns:

    sudo killall cjdroute

Si usted tiene problemas, ejecute `killall cjdroute` para regresar a la tranquilidad. 
Use `pgrep cjdroute` o `top` para ver si el proceso sigue corriendo.

**¡Nota!**

Esto inicia cjdns como el usuario root para que pueda configurar su sistema
sin impedimentos por los permisos. Para iniciar cjdns como un usuario que no
es root, favor de leer [doc/non-root-user.md](doc/non-root-user.md) - en inglés -.


### 6. Acceda a IRC

¡Bienvenido a la red! Ahora es un administrador de redes. Hay responsabilidades
que vienen con ser un administrador de redes, que incluye estar disponible en
caso de que algo este mal con su equipamiento. Debe de estar en [IRC](#community)
de tal manera de que la gente le encuentre.


## Interfaz de administración

Cuando cjdroute esta habilitado y corriendo, la interfaz de administración estara en
`udp://localhost:11234` (esto puede ser cambiado en el archivo de configuracion
`cjdroute.conf`). Consulte [doc/admin-api.md](doc/admin-api.md) - en inglés - para 
mas información sobre la interfaz de administracion. Ha varias herramientas en
la carpeta `contrib/` que puede interactuar con ella.

Puede acceder a la API de administración con:

* La **Python library**; vea [aquí](contrib/python/README.md) - en inglés -.
* La **Perl library**, mantenida por Mikey; vea [aquí](contrib/perl/CJDNS/README) - en inglés -.


## Reportando problemas
1. No hacer reportes en este repositorio, mejor hagalo en https://github.com/hyperboria/bugs/issues
2. Accede a IRC y converse con alguien
3. Lo que pasara en ambos casos
 * Alguien estará de humor para acomodarlo
 * Usted estará de humor para acomodarlo
 * Nadien le importara y sera eventualmente olvidado y talves alguien se topara futuramente con ello,
 lo arregle, o termine superado en alguna reestructura completa del código.
 * Nadie por el momento podrá repararlo pero tendra el merito de ser recordado por su gran significado
 en como el código se desarrolla, y sera el caso de que tendrá que ser explicado por alguien fuertemente
 relacionado al codigo. Podrá realizar una peticion pull al directorio docs/bugs.

### Seguridad
Problemas de seguridad deberán de ser reportado en IRC como cualquier otro bug. No tenemos un grupo
cerrado de personas con conocimientos especiales, lo que significa que el método por defacto para 
reportar cosas de seguridad, es con su descripción completa.
Consulte: https://github.com/cjdelisle/cjdns/blob/master/doc/security_specification.md - en ingles -
para comprobar si un aparente problema de seguridad realmente es un problema de seguridad.

Esa época del año nuevamente... ¡El tiempo para actuar en algunos proyectos de código abierto!
[![Hacktoberfest](https://img.shields.io/badge/Open%20Source-Hacktoberfest-orange.svg)](https://hacktoberfest.digitalocean.com/)

[IRC Web]: http://chat.efnet.org/irc.cgi?chan=%23cjdns
[Hyperboria]: http://hyperboria.net
[Project Meshnet]: https://projectmeshnet.org
[/r/darknetplan]: http://www.reddit.com/r/darknetplan
[#cjdns on Twitter]: https://twitter.com/hashtag/cjdns
[Hyperboria Map]: http://www.fc00.org/
[Buildbots]: https://buildbot.meshwith.me/cjdns/waterfall

[Cjdns on Wikipedia]: https://en.wikipedia.org/wiki/Cjdns
[Distributed Hash Table]: https://en.wikipedia.org/wiki/Distributed_hash_table
[Beyond Pain]: https://lists.torproject.org/pipermail/tor-dev/2012-October/004063.html
[Kademlia]: https://en.wikipedia.org/wiki/Kademlia

[Tor]: https://www.torproject.org
[I2P]: http://www.i2p2.de
[Freenet]: https://freenetproject.org
