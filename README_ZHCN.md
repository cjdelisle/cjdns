
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

#### *重朔整個网络*

Cjdns 利用「加密的IPv6」及「公钥加密」來分配网络地址并利用「Distributed Hash Table」进行路由。它能提供近似「零配置网络（Zero-Configuration Networking）」，並且能防范在現有网络中存在的很多和安全、可拓展性相关的问题。

[![Build Status](https://travis-ci.org/cjdelisle/cjdns.svg?branch=master)](https://travis-ci.org/cjdelisle/cjdns)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/387/badge)](https://bestpractices.coreinfrastructure.org/projects/387)
[![tip for next commit](https://tip4commit.com/projects/941.svg)](https://tip4commit.com/github/cjdelisle/cjdns)
[![irc](https://img.shields.io/badge/irc%20chat-%23cjdns-blue.svg)](https://kiwiirc.com/client/irc.efnet.org/?nick=visitor|?#cjdns)

## 评价

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

## 社群

* [irc://irc.efnet.org/#cjdns][IRC Web]
* [Hyperboria][] 目前最大的 cjdns 网络，至2015年10月止有2100個节点。
* [Project Meshnet][]
* [/r/darknetplan][]
* [#cjdns on Twitter][]


## 相关文件

* [Project Goals](doc/projectGoals.md)
* [Cjdns Whitepaper](doc/Whitepaper.md)
* [Cjdns on Wikipedia][]

进阶配置（英文）:

* [Setup a cjdns NAT gateway for your LAN](doc/nat-gateway.md)
* [Install cjdns on OpenIndiana](doc/open-indiana.md)

感谢您的时间和关注

cjdns 开发人员 敬上

--------------------------------------------------------------------------------

## 如何安裝 cjdns

这些说明虽然是在基于 Debian 的 Linux 发行版系統与 macOS 系统上的，但是已经为了在其它发行版操作系统安装提供了足夠多的信息，只是不要期望可以一字不差地执行指令。

### 0. 安装相关程序

在两种操作系统上安裝 [Node.js](http://nodejs.org/)，非必要但强烈建议。
如无 Node.js 或版本错误，安装程序会自动下载安装包到源代码文件夹中。

#### Debian 系统:

    sudo apt-get install nodejs git build-essential python2.7

#### Fedora 22+ 系统:

    sudo dnf install install nodejs git
    sudo dnf install @development-tools

#### RHEL 系统 (需要新增 EPEL 的 Repository):

    sudo yum localinstall https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
    sudo yum install install nodejs git
    sudo yum install @development-tools

#### macOS 系统:

使用 [Homebrew](https://brew.sh/) 安装:

    brew install cjdns

使用 [MacPorts](https://www.macports.org/) 安装:

    sudo port install cjdns

#### OpenBSD 系统:

非常遗憾目前 OpenBSD 还在实验阶段。

    pkg_add git node gcc gmake bash

请选择 gcc-4.8.1p2 之后版本。

#### FreeBSD 系统:

所有所需的相关程序已经预先在 FreeBSD' ports 编译完成。

    pkg install gmake node

#### Arch 系统:

你可以用以下指令安裝 cjdns

    pacman -S cjdns

如果你需要从源码重新编译，所有需要的程序可以用以下方式安裝

    pacman -S nodejs git base-devel
    
另外，你可能会想利用 AUR 安装包进行安装 `cjdns-git`。
安装完成后，配置文件在 `/etc/cjdroute.conf`。
用以下命令启动 `cjdns.service`：

        systemctl start cjdns

停止命令：

       systemctl stop cjdns

### 1. 从 GitHub 下载 cjdns 源码

从 Github 复制 Repository 并切换至下载文件夹：

    git clone https://github.com/cjdelisle/cjdns.git cjdns
    cd cjdns

### 2. 初始化

    ./do

当你看见 `Build completed successfully, type ./cjdroute to begin setup.`，
再继续以下步骤:

--------------------------------------------------------------------------------

## 安裝

不带参数执行 cjdroute 時会显示帮助信息：

    ./cjdroute

### 0. 确定你已经准备好了必要的资料。

    LANG=C cat /dev/net/tun

如果显示： `cat: /dev/net/tun: File descriptor in bad state` 表示安装成功!

如果显示： `cat: /dev/net/tun: No such file or directory`，利用以下指令新建文件夹：

    sudo mkdir -p /dev/net &&
    sudo mknod /dev/net/tun c 10 200 &&
    sudo chmod 0666 /dev/net/tun

然后再次执行 `cat /dev/net/tun` 。

如果显示： `cat: /dev/net/tun: Permission denied` 你大概在使用一個基于 OpenVZ Virtualization Platform 的 VPS（虚拟服务器）。
向你的服务提供商要求开启 TUN/TAP 设备 - 这是标准协议，所以他們会明白你的意思。

如果你用 macOS 请忽略这個步骤。

### 1. 产生一个新的配置文件（请先阅读下方「保护好你的配置文件」部份）

    ./cjdroute --genconf >> cjdroute.conf

**保护好你的配置文件！ **

失去配置文件代表你丢失密码以及已建立的连结，其他以你做为接入点的节点也会无法与你连线。
配置档被侵入代表他人可以在网络上假装是你。

新建一个仅有你的使用者才有权限读写的配置文件：

    (umask 077 && ./cjdroute --genconf > cjdroute.conf)


### 2. 找一个朋友

为了加入一个已经存在的网络（例如：Hyperboria），你需要与一个已经在该网络中的节点连接。
原因如下：

1. 因为有心人士较不会在「他人好心邀请下加入」后滥用系统。
2. 此程式目的并非要在现有网路之上并行，而是取代现有网路。各个连线会在未来被实体线路或无线网路取代。
3. 如果有任何异议，会有「朋友的朋友」将两方连在一起，这使和解更容易达成
   
要找朋友，请发声并加入我们的 [社群](#社群)。另外，来[Hyperboria Map][] 找寻邻近的节点。

### 3. 连接你与朋友的节点

**由你启始连线（向外）**

在你的配置档，你可以找到：

``` javascript
// Nodes to connect to.
"connectTo":
{
    // Add connection credentials here to join the network
    // Ask somebody who is already connected.
}
```

一個包含许多朋友节点（向外）的配置文件类似这样：

``` javascript
// Nodes to connect to.
"connectTo":
{
    //朋友_1 (IPv4: 0.1.2.3; IPv6 fcaa:5bac:66e4:713:cb00:e446:c317:fc39)
    "0.1.2.3:45678":
    {
        "login": "k.alexander"
        "password": "这不是真的连线_1",
        "publicKey": "这只是范例请不要真的在配置档中用这串字_1.k"
    }

    //朋友_2 (IPv4: 5.1.2.3; IPv6 fcbb:5bac:66e4:713:cb00:e446:c317:fc39)
    "5.1.2.3:5678":
    {
        "login": "k.alexander"
        "password": "这不是真的连线_2",
        "publicKey": "这只是范例请不要真的在配置档中用这串字_2.k"
    }
}
```

你可以依照 JSON 语法在 `connectTo` 属性中加入无限个连线。


**让你的朋友启始连线（向内）**

在配置档中，你可以找到：

``` javascript
"authorizedPasswords":
[
    // A unique string which is known to the client and server. 
    {"password": "独一无二的密码（请不要输入中文）_001"}

    // More passwords should look like this. 
    // {"password": "独一无二的密码（请不要输入中文）_002"}
    // {"password": "独一无二的密码（请不要输入中文）_003"}
    // {"password": "独一无二的密码（请不要输入中文）_004"}
    ...

    // "你的.对外.IP.位址:45678":{"password": "独一无二的密码（请不要输入中文）_001","publicKey":独一无二的金钥_001.k"}

],
```

一个有许多朋友节点的配置档，向内连线应该长这样：
``` javascript
"authorizedPasswords":
[
    // A unique string which is known to the client and server. 在此输入让他人对你连线的密码
    {"password": "独一无二的密码（请不要输入中文）_001", "user": "k.alexander"}

    // More passwords should look like this. 其他请依此格式类推
    //William Jevons (IPv4: 0.1.2.3; IPv6 fcaa:5bac:66e4:713:cb00:e446:c317:fc39)
    {"password": "独一无二的密码（请不要输入中文）_002", "user": "William Jevons"}
    //Marilyn Patel (IPv4: 5.1.2.3; IPv6 fcbb:5bac:66e4:713:cb00:e446:c317:fc39)
    {"password": "独一无二的密码（请不要输入中文）_003", "user": "Marilyn Patel"}
    // {"password": "独一无二的密码（请不要输入中文）_004"}
    ...

    // "你的.对外.IP.位址:45678":{"password": "独一无二的密码（请不要输入中文）_001","publicKey":独一无二的金钥_001.k"}
],
```
你必须给 William Jevons (对你而言是向内连线) 下方四项资讯：

1. 你的外部 IPv4
2. 你可以在配置档找到的埠号：

    `// Bind to this port.
    "bind": "0.0.0.0:你的埠号在这里",`

3. 为你朋友设定或取消注解的唯一密码： `"password": "独一无二的密码（请不要输入中文）_002"`
4. 你的公钥： `"publicKey": "独一无二的金钥_001.k"`
5. 他的登入名称： "William Jevons"

他的配置档中会看起来像这样(包含你的 IPv4 和 埠号):

```javascript
"1.2.3.4:56789": {
    "login": "William Jevons",
    "password": "独一无二的密码（请不要输入中文）_002",
    "publicKey": "这只是范例请不要真的在配置档中用这串字_1.k"
}
```

请记得你与你朋友可以启始「向外（你 --> 朋友）」或「向内（朋友 --> 你）」连线，但当通道建立后两方皆可使用。

更进阶的配置请阅读 [doc/configure.md](doc/configure.md)，包括如何透过乙太网路与Wifi连线。

### 4. 保护你的系统 - 检查正在使用网路的程式

当你的节点开始运行，你的电脑便成为一个新的 IPv6 伺服。你的作业系统可能会自动重新配置网路程式来使用这个新的网路位址。
如果你并不希望这么做，你应确认你并没有提供一些你不打算公开的服务。 ;)

更多资料请查阅 [doc/network-services.md](doc/network-services.md)。


### 5. 开始使用！

    sudo ./cjdroute < cjdroute.conf

如果你希望将纪录存成一个档案：

    sudo ./cjdroute < cjdroute.conf > cjdroute.log

停止 cjdns:

    sudo killall cjdroute

如果出现问题用 `killall cjdroute` 指令来结束一切。
用 `pgrep cjdroute` 或 `top` 指令来确认是否正在执行。

**注意！ **

这将会让 cjdns 以 Root 权限执行并设定你的系统，避免要求权限等提示。请参阅[doc/non-root-user.md](doc/non-root-user.md) 来以非 Root 权限执行。

### 6. 加入 IRC

欢迎来到这个网络！你现在是一个网管了。这代表你有些成一个网管的责任，包括当你的设备出状况时必须能回应。你应当在 [IRC](#community) 保持在线，这样其他人才能与你联系。


## 管理者界面

当 cjdroute 正在执行时，你可以在 `udp://localhost:11234` 看到管理者界面 (这可以在 `cjdroute.conf` 更改). 这个档案 [doc/admin-api.md](doc/admin-api.md) 有更多有关管理者界面的资讯。一些在 `contrib/` 的工具可以配合使用。

你可以透过以下方式使用 Admin API ：

* **Python library**; 按 [这里](contrib/python/README.md).
* **Perl library**, 由 Mikey 维护; 按 [这里](contrib/perl/CJDNS/README).

## 报告问题
1. 请不要在此报告问题，有问题请到 https://github.com/hyperboria/bugs/issues 报告
2. 在 IRC 上讲
3. 以下是可能发生的
 * 有人主动来解决问题
 * 你主动来解决问题
 * 根本没人理这问题然后被遗忘，也许某天有人会遇到同样的问题然后在某次的重构中解决
 * 目前无解但是应该被记住，因为这会影响未来程式开发，这种情况必须被对程式码有深入理解的人进行解释。他会提出对 Bug 或文件进行修改

### 安全
安全问题应该同样与其他 Bug 一样在 IRC 报告。我们并没有一个拥有特殊技术的封闭团队，所以预设的安全问题回报是完全公开。
请参阅： https://github.com/cjdelisle/cjdns/blob/master/doc/security_specification.md 来确认可能安全问题是真的安全问题。

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
