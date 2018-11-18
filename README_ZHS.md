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

#### *网络之再创造*

Cjdns实现了一个加密的IPv6网络，使用了公钥密码系统用于地址分配，和分布式哈希表用于路由。这提供了几乎零配置的网络，并避免了很多烦扰现有网络的安全和扩展性问题。

[![Build Status](https://api.travis-ci.org/cjdelisle/cjdns.svg?branch=master)](https://travis-ci.org/cjdelisle/cjdns)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/387/badge)](https://bestpractices.coreinfrastructure.org/projects/387)
[![tip for next commit](https://tip4commit.com/projects/941.svg)](https://tip4commit.com/github/cjdelisle/cjdns)
[![irc](https://img.shields.io/badge/irc%20chat-%23cjdns-blue.svg)](https://kiwiirc.com/client/irc.efnet.org/?nick=visitor|?#cjdns)
![License](https://img.shields.io/github/license/cjdelisle/cjdns.svg)

## 赞许

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

## 社区

* [irc://irc.efnet.org/#cjdns][IRC网页版]
* [Hyperboria][] 最大的 cjdns 网络，在 2015年10月 有 2100 个节点。
* [/r/darknetplan][]
* [#cjdns于Twitter][]


## 文档

* [项目目标](doc/projectGoals.md)
* [Cjdns白皮书](doc/Whitepaper.md)
* [Cjdns于维基百科][]

进阶配置：

* [为你的局域网配置cjdns NAT网关](doc/nat-gateway.md)
* [在OpenIndiana上安装cjdns](doc/open-indiana.md)

感谢你付出的时间和关注，

cjdns开发者。

--------------------------------------------------------------------------------

## 如何安装cjdns

这些指导针对Debian系的Linux发行版和macOS系统。它们应该为其他发行版提供
足够的安装步骤信息 - 别期望一字不差地拷贝命令。 如果你想知道为什么 [操纵系统基础在这儿](https://upload.wikimedia.org/wikipedia/commons/1/1b/Linux_Distribution_Timeline.svg).

### 0. 安装依赖

在任意一个系统中，安装 [Node.js](https://nodejs.org/)，是推荐的
但不是必须的。如果 Node.js 不可用或是一个不可接受的版本，
它将自动下载并安装到源码树。

#### Debian 系发行版：

    sudo apt-get install nodejs git build-essential python2.7

#### Fedora 22+ 系发行版：

    sudo dnf install nodejs git
    sudo dnf install @development-tools

#### RHEL 系发行版（添加 EPEL 仓库）：

    sudo yum localinstall https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
    sudo yum install nodejs git
    sudo yum install @development-tools

#### 从源码包构建：

    sudo yum localinstall https://kojipkgs.fedoraproject.org//packages/cjdns/17.4/4.el6/src/cjdns-17.4-4.el6.src.rpm

如果你在笔记本上操作并且挂起或是休眠，当醒来时 cjdroute 将花费几分钟“煮咖啡”并且弄清楚刚才发生了什么。
你可以这样去加速这个过程：

    systemctl enable cjdns-resume

这个恢复服务当系统从睡眠中醒来时重启 cjdns 。

#### Gentoo

    emerge --ask nodejs sys-devel/gcc dev-lang/python:3.4 dev-vcs/git

#### macOS：

用 [Homebrew](https://brew.sh/) 安装：

    brew install cjdns

用 [MacPorts](https://www.macports.org/) 安装：

    sudo port install cjdns

#### OpenBSD：

    pkg_add git node gcc gmake bash

选择版本 gcc-4.8.1p2 或是更新的。

#### FreeBSD：

你所需要的全部在 FreeBSD的 ports 中有预编译的了。

    pkg install gmake node

#### Arch：

你可以安装 cjdns 通过运行

    pacman -S cjdns

如果你需要从源码构建，你需要的全部能像这样去安装

    pacman -S nodejs git base-devel

另外，你可能喜欢从包的AUR安装， `cjdns-git`.
安装后，配置文件存放在 `/etc/cjdroute.conf`.
要开启服务 `cjdns.service`, 执行：

    systemctl start cjdns

要停止它：

    systemctl stop cjdns

#### Gentoo：

cjdns 还不在Gentoo主仓库内，所以你得使用个overlay。
最简单的方法是使用 Layman 但是你也能手动去做。

##### Layman：

首先，你需要安装layman。

    emerge layman

如果 layman 安装正常，你可以添加 overlay

    layman -f
    layman -a weuxel

未来升级 overlay 的时候用

    layman -S

现在你可以安装 cjdns

    emerge cjdns

##### 手动：

你必须得 clone overlay 仓库

    cd /opt
    git clone https://github.com/Weuxel/portage-weuxel.git

再告诉 portage 使用这个仓库

    cd /etc/portage/repos.conf/

创建一个文件 `portage-weuxel.conf` 包含

    [weuxel]
    location = /opt/portage-weuxel
    masters = gentoo
    auto-sync = yes

现在 sync

    emerge --sync

并安装 cjdns

    emerge cjdns

#### 自动崩溃检测并重启

从 `contrib/openrc` 复制 OpenRC 初始化脚本到 `/etc/init.d/` 并根据你的需要修改 `CONFFILE` 和 `command` 参数。
然后启动 cjdns 通过执行

    /etc/init.d/cjdns start

配置初始化系统自动启动 cjdns

    rc-update add cjdns default

复制 service_restart 脚本 `contrib/gentoo/service_restart.sh` 到你系统中任意方便的目录，
然后修改 eMail 地址。如果你不想被提醒，注释掉整行。
现在添加一个 crontab 条目像这样

    # Restart crashed Services
    * * * * *       root	/path/to/script/service_restart.sh

#### Solus：

安装依赖：

    sudo eopkg install nodejs git build-essential system.devel python gcc binutils kernal-headers xorg-server-devel

接下来按以下步骤：

* 抱歉这么多步。我们当前的工作是建立软件包以方便安装 *

##### 1. 从 GitHub 获取 cjdns

从 GitHub clone 仓库并切换到源码目录：

    git clone https://github.com/cjdelisle/cjdns.git cjdns
    cd cjdns

##### 2. 构建

    ./do

等待出现 `Build completed successfully, type ./cjdroute to begin setup.`, 接下来按下面的继续：

--------------------------------------------------------------------------------

## 安装

无参数运行 cjdroute 获取帮助：

    ./cjdroute

### 0. 确保你已经准备好设备。

如果你在 macOS ，别担心这一步。

    LANG=C cat /dev/net/tun

如果出现： `cat: /dev/net/tun: File descriptor in bad state` 一切正常!

如果出现： `cat: /dev/net/tun: No such file or directory`，创建它使用：

    sudo mkdir -p /dev/net &&
    sudo mknod /dev/net/tun c 10 200 &&
    sudo chmod 0666 /dev/net/tun

之后再次 `cat /dev/net/tun` 。

如果出现： `cat: /dev/net/tun: Permission denied` 你可能使用了一个基于 OpenVZ 虚拟化平台的 VPS。
请求你的提供商开启 TUN/TAP 设备 - 这是一个标准协议所以他们应该确切知道你应该需要什么。


### 1. 生成一个新的配置文件

    ./cjdroute --genconf >> cjdroute.conf

**保护你的配置文件!**

丢失配置文件意味着丢失你的密码和连接，任何连接到你的人再也不能够连接到你。
一个泄漏的配置文件意味着其他人能在网络上冒充你。

生成一个包含权限集的配置文件使得只有你自己这个用户才能读写它：

    (umask 077 && ./cjdroute --genconf > cjdroute.conf)


### 2. 找个朋友

要接入已有的网络（例如 Hyperboria），你需要连接到
已经在网络中的某人。这是必要的因为一些原因：

1. 帮助避免滥用，因为少有坏人在他人热心帮助下进入系统后破坏系统。
2. 这个项目不打算叠加到旧的因特网上，而意图是取代它。每个连接将在未来被网线、光纤或是无线网络取代。
3. 在任何异议的情况下，有一个“朋友链条”将参与的人们连接起来为达成共识形成一个基础。

要找朋友，从这出来然后加入我们的 [社区](#社区)。另外，
看一看 [Hyperboria Map][] 找到你附近的对等节点。


### 3. 把你的节点连接到朋友的节点

**初始化 OUTbound 连接**

在你的配置文件，你会看到：

``` javascript
// Nodes to connect to.
"connectTo":
{
    // Add connection credentials here to join the network
    // Ask somebody who is already connected.
}
```

一个有多个朋友节点的配置文件，设置 OUTbound, 应该像这样：

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

你可以添加任意多的连接到 `connectTo` 属性，按照 JSON 语法。


**允许你的朋友初始化 INbound 连接**

在你的配置文件里，你可以看到：

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

一个有多个朋友节点的配置文件，设置 INbound, 应该像这样：
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


你需要给 William Jevons（建立 INbound 连接的人）以下4条：

1. 你的外部 IPv4
2. 你的配置文件里面这样的端口号：

    `// Bind to this port.
    "bind": "0.0.0.0:yourportnumberishere",`

3. 你取消注释或创建的他们的独立密码： `"password": "thisisauniquestring_002"`
4. 你的公钥： `"publicKey": "thisisauniqueKEY_001.k"`
5. 他的用户名： "William Jevons"

他的登录凭证看起来像这样（用你的 IPv4 和端口）：

```javascript
"1.2.3.4:56789": {
    "login": "William Jevons",
    "password": "thisisauniquestring_002",
    "publicKey": "thisIsJustForAnExampleDoNotUseThisInYourConfFile_1.k"
}
```

请注意你和你的朋友可以*初始化*一个连接以
outbound（从 你 --> 朋友） 或 inbound（从 朋友 --> 你）的方式之一，
但是一旦连接建立后流量是双向的。

见 [doc/configure.md](doc/configure.md) 获取配置上的更多细节，
包括怎样通过以太网和 WiFi 连接其他 cjdns 节点。


### 4. 加固你的系统 - 检查侦听的服务

当你的节点运行后，你现在是一个新的IPv6主机。你的操作系统可以自动配置网络服务以使用这个新地址。
如果这不是你所期望的，你应该检查看是否你没有比你所期望的提供了更多的网络服务。 ;)

见 [doc/network-services.md](doc/network-services.md) 获取指南。


### 5. 启动！

    sudo ./cjdroute < cjdroute.conf

如果你想把日志写入文件：

    sudo ./cjdroute < cjdroute.conf > cjdroute.log

要停止 cjdns：

    sudo killall cjdroute

如果你遇到问题，使用 `killall cjdroute` 强行恢复。
使用 `pgrep cjdroute` 或 `top` 观察是否在运行。

**注意!**

以 root 用户开启 cjdns ，这样能够无需关系权限来配置你的系统。
要以非 root 用户开启 cjdns ，见 [doc/non-root-user.md](doc/non-root-user.md).


### 6. 加入 IRC

欢迎来到网络！你现在是一个网络管理员了。你有作为网络管理员的责任，包括当你的设备出现一些问题的时候你应该在线。
你应该呆在 [IRC](#community) 这样人们能找到你。


## 管理员界面

当 cjdroute 启动并运行时，管理员界面在 `udp://localhost:11234`( 这可以在配置文件 `cjdroute.conf` 中修改 )。
见 [doc/admin-api.md](doc/admin-api.md) 获取关于管理员界面的更多信息。
在 `contrib/` 里有几个工具可以使用。

你可以访问管理员 API 通过：

* **Python 库**; 见 [这儿](contrib/python/README.md)。
* **Perl 库**, 由 Mikey 维护; 见 [这儿](contrib/perl/CJDNS/README)。


## 报告问题
1. 别在这儿报告，请到 https://github.com/hyperboria/bugs/issues 。
2. 登上 IRC 与人交谈。
3. 可能发生以下结果之一
 * 某人愿意修复它。
 * 你想修复它。
 * 没人理它然后它被遗忘一段时间，接下来可能有人重新提起并修复它或者它将被在重构时清理。
 * 目前没人可以修复但它应被记下，因为它对代码发展的方向有重大的影响，
 这种情况下它需要被熟悉代码的人用术语解释清楚。他们将将发起一个 pull request 到文档/Bug目录。

### 安全
安全问题应该在像其他 Bug 一样在 IRC 上报告。
我们内部没有一个有专业知识的团队，这意味着默认的安全报告方式是完全对外公开的。

见： [security_specification.md](https://github.com/cjdelisle/cjdns/blob/master/doc/security_specification.md) 来确认是否一个潜在的安全问题是真正的安全问题。

又是一年这时候…… 一些开源项目的时间到啦！
[![Hacktoberfest](https://img.shields.io/badge/Open%20Source-Hacktoberfest-orange.svg)](https://hacktoberfest.digitalocean.com/)

[IRC网页版]: http://chat.efnet.org/irc.cgi?chan=%23cjdns
[Hyperboria]: https://hyperboria.net
[/r/darknetplan]: https://www.reddit.com/r/darknetplan
[#cjdns于Twitter]: https://twitter.com/hashtag/cjdns
[Hyperboria Map]: https://www.fc00.org/
[Buildbots]: https://buildbot.meshwith.me/cjdns/waterfall

[Cjdns于维基百科]: https://en.wikipedia.org/wiki/Cjdns
[分布式哈希表]: https://en.wikipedia.org/wiki/Distributed_hash_table
[Beyond Pain]: https://lists.torproject.org/pipermail/tor-dev/2012-October/004063.html
[Kademlia]: https://en.wikipedia.org/wiki/Kademlia

[Tor]: https://www.torproject.org
[I2P]: https://geti2p.net/en/
[Freenet]: https://freenetproject.org
