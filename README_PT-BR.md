# cjdns

[English](README.md)
[Русская версия](README_RU.md)
[Hrvatski](README_HR.md)
[Svenska](README_SV.md)
[Ελληνικά](README_GR.md)
[Deutsch](README_DE.md)
[繁體中文](README_ZHT.md)
[Español](README_ES.md)
[Français](README_FR.md)
[Português brasileiro](README_PT-BR.md)

#### *Networking Reinventada*

O Cjdns implementa uma rede IPv6 criptografada usando criptografia de chave pública para
alocação de endereços e uma tabela de hash distribuída para roteamento. Isso fornece
configuração de rede quase zero e evita muitos dos aspectos de segurança e
problemas de escalabilidade que afetam as redes existentes.

[![Build Status](https://api.travis-ci.org/cjdelisle/cjdns.svg?branch=master)](https://travis-ci.org/cjdelisle/cjdns)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/387/badge)](https://bestpractices.coreinfrastructure.org/projects/387)
[![tip for next commit](https://tip4commit.com/projects/941.svg)](https://tip4commit.com/github/cjdelisle/cjdns)
[![irc](https://img.shields.io/badge/irc%20chat-%23cjdns-blue.svg)](https://kiwiirc.com/client/irc.efnet.org/?nick=visitor|?#cjdns)
![License](https://img.shields.io/github/license/cjdelisle/cjdns.svg)

## Depoimentos

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

## Comunidade

* [irc://irc.efnet.org/#cjdns][IRC Web]
* [Hyperboria][] a maior rede de cjdns, em outubro de 2015, havia 2100 nós.
* [/r/darknetplan][]
* [#cjdns no Twitter][]
* [/r/CJDNS](https://www.reddit.com/r/cjdns/)


## Documentação

* [Objetivos do projeto](doc/projectGoals.md)
* [Cjdns Whitepaper](doc/Whitepaper.md)
* [Cjdns na Wikipedia][]

Configuração avançada:

* [Configure um gateway NAT cjdns para sua LAN](doc/nat-gateway.md)
* [Instale cjdns no OpenIndiana](doc/open-indiana.md)

Obrigado por seu tempo e interesse,

Os desenvolvedores do cjdns.

--------------------------------------------------------------------------------

## Como instalar o cjdns

Estas instruções são para distribuições Linux baseadas no Debian e macOS. Eles deveriam ser
informativo o suficiente para uso em outras distribuições - apenas não espere que
trabalhar literalmente. Se você quer saber qual é a [base do sistema operacional, clique aqui](https://upload.wikimedia.org/wikipedia/commons/1/1b/Linux_Distribution_Timeline.svg).

### 0. Instalar dependências

Nas duas plataformas, a instalação do [Node.js] (https://nodejs.org/), embora seja preferível,
não é estritamente necessário. Se o Node.js estiver indisponível ou uma versão inaceitável,
ele será baixado e instalado na árvore de origem.

#### Distribuição baseada no Debian:

    sudo apt-get install nodejs git build-essential python2.7

#### Distro baseado no Fedora 22+:

    sudo dnf install nodejs git
    sudo dnf install @development-tools

#### Distribuição baseada em RHEL (adiciona o repositório EPEL):

    sudo yum localinstall https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
    sudo yum install nodejs git
    sudo yum install @development-tools

#### Construção a partir do pacote:

    sudo yum localinstall https://kojipkgs.fedoraproject.org//packages/cjdns/17.4/4.el6/src/cjdns-17.4-4.el6.src.rpm

Se você estiver em um laptop e o suspender ou hibernar, o cjdroute levará alguns
minutos para fazer café e descobrir o que aconteceu quando acorda. Vocês
pode acelerar drasticamente isso com:

    systemctl enable cjdns-resume

O serviço de reinicialização reinicia o cjdns quando o sistema acorda do modo de suspensão.

#### Gentoo

    emerge --ask nodejs sys-devel/gcc dev-lang/python:3.4 dev-vcs/git

#### macOS:

Instale com [Homebrew](https://brew.sh/):

    brew install cjdns

Instale com [MacPorts](https://www.macports.org/):

    sudo port install cjdns

#### OpenBSD:

    pkg_add git node gcc gmake bash

Selecione a versão gcc-4.8.1p2 ou mais recente.

#### FreeBSD:

Tudo o que você precisa está disponível pré-compilado nas portas do FreeBSD.

    pkg install gmake node

#### Arch:

Você pode instalar cjdns executando:

    pacman -S cjdns

Se você precisa construir a partir do código-fonte, tudo o que você precisa pode ser instalado assim:

    pacman -S nodejs git base-devel

Como alternativa, você pode instalar via AUR a partir do pacote, `cjdns-git`.
Após a instalação, o arquivo de configuração está localizado em `/etc/cjdroute.conf`.
Para iniciar o serviço `cjdns.service`, faça:

    systemctl start cjdns

Para parar:

    systemctl stop cjdns

#### Gentoo:

O cjdns ainda não está no repositório principal do Gentoo, então você terá que usar uma sobreposição.
A maneira mais fácil é usar o Layman, mas você também pode fazê-lo manualmente.

##### Layman:

Primeiro, você precisa instalar layman.

    emerge layman

Se o layman estiver instalado corretamente, você poderá adicionar a sobreposição

    layman -f
    layman -a weuxel

Para atualização futura da sobreposição, use

    layman -S

Agora você pode instalar o cjdns

    emerge cjdns

##### À mão:

Você precisará clonar o repositório de sobreposições

    cd /opt
    git clone https://github.com/Weuxel/portage-weuxel.git

Agora diga ao portage para usar este repositório

    cd /etc/portage/repos.conf/

Crie um arquivo `portage-weuxel.conf` contendo

    [weuxel]
    location = /opt/portage-weuxel
    masters = gentoo
    auto-sync = yes

Agora sincronize

    emerge --sync

E instale cjdns

    emerge cjdns

#### Detecção e reinício automáticos de falhas

Copie o script init openrc de `contrib/openrc` para `/etc/init.d/` e modifique o `CONFFILE` e `command` parâmetro para suas necessidades.
Em seguida, inicie o cjdns emitindo

    /etc/init.d/cjdns start

Configure o sistema init para iniciar automaticamente cjdns

    rc-update add cjdns default

Copie o script service_restart `contrib/gentoo/service_restart.sh` para qualquer diretório conveniente em
seu sistema e modifique o endereço de e-mail. Se você não deseja ser notificado, comente toda a linha.
Agora adicione uma entrada crontab como esta

    # Restart crashed Services
    * * * * *       root	/path/to/script/service_restart.sh

#### Solus:

Dependências:

    sudo eopkg install nodejs git build-essential system.devel python gcc binutils kernal-headers xorg-server-devel

Siga as etapas abaixo:

* Desculpe por tantas etapas. Um pacote está sendo trabalhado atualmente *

##### 1. Recuperar cjdns do GitHub

Clone o repositório do GitHub e mude para o diretório de origem:

    git clone https://github.com/cjdelisle/cjdns.git cjdns
    cd cjdns

##### 2. Construir

    ./do

Procure por `Build completed successfully, type ./cjdroute to begin setup.`e, em seguida,
prossiga abaixo:

--------------------------------------------------------------------------------

## Configuração

Execute cjdroute sem opções para HELP:

    ./cjdroute

### 0. Verifique se você tem as coisas.

Se você estiver no macOS, não se preocupe com esta etapa.

    LANG=C cat /dev/net/tun

Se diz: `cat: /dev/net/tun: File descriptor in bad state` Bom!

Se diz: `cat: /dev/net/tun: No such file or directory`, crie usando:

    sudo mkdir -p /dev/net &&
    sudo mknod /dev/net/tun c 10 200 &&
    sudo chmod 0666 /dev/net/tun

Então `cat /dev/net/tun` novamente.

Se diz: `cat: /dev/net/tun: Permission denied` Você provavelmente está usando um VPS
com base na plataforma de virtualização OpenVZ. Peça ao seu provedor para ativar o
Dispositivo TUN / TAP - este é o protocolo padrão para que eles saibam exatamente o que você
necessita.


### 1. Gere um novo arquivo de configuração

    ./cjdroute --genconf >> cjdroute.conf

**Proteja seu arquivo conf!**

Um arquivo conf perdido significa que você perdeu sua senha e
conexões e qualquer pessoa conectada a você não poderá mais se conectar.
Um arquivo conf comprometido significa que outras pessoas podem se passar por você na rede.

Para gerar um arquivo conf com permissões definidas, para que somente seu usuário possa lê-lo e gravá-lo:

    (umask 077 && ./cjdroute --genconf > cjdroute.conf)


### 2. Encontre um amigo

Para entrar em uma rede existente (por exemplo, Hyperboria), você precisa se conectar a alguém que já esteja na rede. Isso é necessário por vários motivos:

1. Ajuda a evitar abusos, porque as pessoas más terão menos probabilidade de abusar de um sistema depois de terem, em um ato de bondade humana, acesso a esse sistema.
2. Não se destina a sobrepor a Internet antiga, mas a substituí-la. No momento oportuno, cada conexão será substituída por um fio, um cabo de fibra ótica ou uma conexão de rede sem fio.
3. Em qualquer caso de desacordo, haverá uma "cadeia de amigos" ligando as pessoas envolvidas, para que já haja uma base para chegar a uma resolução.

Para encontrar um amigo, vá lá e participe de nossa [comunidade] (# comunidade). Além disso, dê uma olhada no [Mapa da Hyperboria] (https://www.fc00.org/) para encontrar colegas perto de você.

Você também pode usar a lista geograficamente variada de credenciais de emparelhamento público para ingressar no Hyperboria em [hyperboria / peers] (https://github.com/hyperboria/peers).

### 3. Conecte seu nó ao nó do seu amigo

**Para iniciar a conexão OUTbound**

No seu arquivo conf, você verá:

``` javascript
// Nodes to connect to.
"connectTo":
{
    // Add connection credentials here to join the network
    // Ask somebody who is already connected.
}
```

Um arquivo conf com vários nós amigos, configuração OUTbound, deve se parecer com:

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
    },

    //friend_2 (IPv4: 5.1.2.3; IPv6 fcbb:5bac:66e4:713:cb00:e446:c317:fc39)
    "5.1.2.3:5678":
    {
        "login": "k.alexander"
        "password": "thisIsNotARealConnection_2",
        "publicKey": "thisIsJustForAnExampleDoNotUseThisInYourConfFile_2.k"
    }
}
```

Você pode adicionar quantas conexões desejar ao atributo `connectTo`,
seguindo a sintaxe JSON.


**Para permitir que seu amigo inicie a conexão INbound**

No seu arquivo conf, você verá:

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

Um arquivo conf com vários nós amigos, configuração INbound, deve se parecer com:
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


Você precisa fornecer a William Jevons (que está fazendo a conexão de entrada) os seguintes 4 itens:

1. Seu IPv4 externo
2. A porta encontrada no seu arquivo conf aqui:

    `// Bind to this port.
    "bind": "0.0.0.0:yourportnumberishere",`

3. A senha exclusiva deles que você descomentou ou criou: `"password": "thisisauniquestring_002"`
4. Sua chave pública: `"publicKey": "thisisauniqueKEY_001.k"`
5. O nome de usuário dele: "William Jevons"

Suas credenciais de login terão a seguinte aparência (com seu IPv4 e porta):

```javascript
"1.2.3.4:56789": {
    "login": "William Jevons",
    "password": "thisisauniquestring_002",
    "publicKey": "thisIsJustForAnExampleDoNotUseThisInYourConfFile_1.k"
}
```

Observe que você e seu amigo podem * iniciar * uma conexão de saída (de VOCÊ -> AMIGO) ou de entrada (de AMIGO -> VOCÊ), mas o tráfego flui para os dois lados quando a conexão é estabelecida.

Consulte [doc/configure.md](doc/configure.md) para obter mais detalhes sobre configuração,
incluindo como fazer pares com outros nós cjdns por Ethernet e Wi-Fi.


### 4. Proteja seu sistema - verifique se há serviços de escuta

Depois que o nó estiver em execução, você será um host IPv6 recém-criado. Seu sistema operacional pode reconfigurar automaticamente os serviços de rede para usar esse novo endereço.
Se não é isso que você pretende, verifique se não está
oferecendo mais serviços do que você pretendia. ;)

Consulte [doc/network-services.md](doc/network-services.md) para obter instruções.


### 5. Iniciar!

    sudo ./cjdroute < cjdroute.conf

Se você deseja que seus logs sejam gravados em um arquivo:

    sudo ./cjdroute < cjdroute.conf > cjdroute.log

Para parar cjdns:

    sudo killall cjdroute

Se você estiver tendo problemas, use `killall cjdroute` para retornar à sanidade. Use
`pgrep cjdroute` ou `top` para ver se está rodando.

**NOTA!**

Isso inicia o cjdns como usuário root, para que ele possa configurar seu sistema sem se preocupar com permissões. Para iniciar o cjdns como um usuário não root, consulte
[doc/non-root-user.md](doc/non-root-user.md).


### 6. Entre no IRC

Bem-vindo à rede! Agora você é um administrador de rede. Existem responsabilidades inerentes ao administrador da rede, que incluem a disponibilidade, caso haja algo errado com o seu equipamento. Você deve permanecer no [IRC](#community) para que as pessoas possam alcançá-lo.

## Interface de administração

Quando o cjdroute estiver em funcionamento, a interface administrativa estará disponível em
`udp://localhost:11234` (isso pode ser alterado no arquivo de configuração `cjdroute.conf`). Consulte [doc/admin-api.md](doc/admin-api.md) para mais
informações sobre a interface de administração. Existem várias ferramentas no `contrib/`
que pode interagir com ele.

Você pode acessar a API do administrador com:

* o **Python library**; consulte [aqui](contrib/python/README.md).
* o **Perl library**, mantido por Mikey; consulte [aqui](contrib/perl/CJDNS/README).


## Relatar problemas
1. Não reporte neste repositório, em vez disso, https://github.com/hyperboria/bugs/issues
2. Entre no IRC e converse com alguém
3. O que vai acontecer é
  * Alguém quer consertar
  * Você quer consertá-lo
  * Ninguém se importa com isso e será esquecido por um tempo e talvez alguém o acerte mais tarde
  e conserte-o, caso contrário ele será apagado em uma refatoração
  * Ninguém pode consertá-lo no momento, mas é considerado digno de lembrança, porque possui ótimas
  importante para a maneira como o código é desenvolvido, nesse caso, ele precisa ser explicado em detalhes técnicos.
  termos por alguém com forte familiaridade com o código. Eles farão uma solicitação de recebimento para o
  diretório docs/bugs.

### Segurança
Os problemas de segurança devem ser relatados no IRC da mesma forma que outros bugs. Não temos um fechado
grupo de pessoas com conhecimentos especiais, o que significa que o método padrão de relatório de segurança é
transparência completa.

Consulte: [security_specification.md](https://github.com/cjdelisle/cjdns/blob/master/doc/security_specification.md) para ver se um possível problema de segurança é realmente um problema de segurança.

Naquela época do ano novamente ... Hora de alguns projetos de código aberto!
[![Hacktoberfest](https://img.shields.io/badge/Open%20Source-Hacktoberfest-orange.svg)](https://hacktoberfest.digitalocean.com/)

[IRC Web]: http://chat.efnet.org/irc.cgi?chan=%23cjdns
[Hyperboria]: https://hyperboria.net
[/r/darknetplan]: https://www.reddit.com/r/darknetplan
[#cjdns no Twitter]: https://twitter.com/hashtag/cjdns
[Hyperboria Map]: https://www.fc00.org/
[Buildbots]: https://buildbot.meshwith.me/cjdns/waterfall

[Cjdns na Wikipedia]: https://en.wikipedia.org/wiki/Cjdns
[Distributed Hash Table]: https://en.wikipedia.org/wiki/Distributed_hash_table
[Beyond Pain]: https://lists.torproject.org/pipermail/tor-dev/2012-October/004063.html
[Kademlia]: https://en.wikipedia.org/wiki/Kademlia

[Tor]: https://www.torproject.org
[I2P]: https://geti2p.net/en/
[Freenet]: https://freenetproject.org
