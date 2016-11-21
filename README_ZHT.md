
# cjdns

[English](README.md)
[Русская версия](README_RU.md)
[Hrvatski](README_HR.md)
[Svenska](README_SV.md)
[Ελληνικά](README_GR.md)
[Deutsch](README_DE.md)

#### *重朔整個網路*

Cjdns 利用「加密的IPv6」及「公鑰加密」來分配網路地址並利用「Hash Table」逕行路由。這近達成「Zero-Configuration Networking」同時避免許多目前網路許多擴展性問題。

[![Build Status](https://travis-ci.org/cjdelisle/cjdns.svg?branch=master)](https://travis-ci.org/cjdelisle/cjdns)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/387/badge)](https://bestpractices.coreinfrastructure.org/projects/387)
[![tip for next commit](https://tip4commit.com/projects/941.svg)](https://tip4commit.com/github/cjdelisle/cjdns)
[![irc](https://img.shields.io/badge/irc%20chat-%23cjdns-blue.svg)](https://kiwiirc.com/client/irc.efnet.org/?nick=visitor|?#cjdns)

## 評價

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
* [Hyperboria][] 目前最大的 cjdns 網路，至2015年10月止有2100個節點。
* [Project Meshnet][]
* [/r/darknetplan][]
* [#cjdns on Twitter][]


## 相關文件

* [Project Goals](doc/projectGoals.md)
* [Cjdns Whitepaper](doc/Whitepaper.md)
* [Cjdns on Wikipedia][]

進階配置（英文）:

* [Setup a cjdns NAT gateway for your LAN](doc/nat-gateway.md)
* [Install cjdns on OpenIndiana](doc/open-indiana.md)

感謝您所貢獻的時間與精力

cjdns 開發人員 敬上

--------------------------------------------------------------------------------

## 如何安裝 cjdns

這些步驟以 Linux Debian 系統與 OS X 系統為主，
相關概念在其他系統上應該足以通用（不過請不要期待可以逐句貼上）。

### 0. 安裝相關程式

在任意系統安裝 [Node.js](http://nodejs.org/)，非必要但建議安裝
如無 Node.js 或版本錯誤，程式會自動下載安裝至 Source Tree 中。

#### Debian 系統:

    sudo apt-get install nodejs git build-essential python2.7

#### Fedora 22+ 系統:

    sudo dnf install install nodejs git
    sudo dnf install @development-tools

#### RHEL 系統 (會新增 EPEL 的 Repository):

    sudo yum localinstall https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
    sudo yum install install nodejs git
    sudo yum install @development-tools

#### OS X 系統:

利用 homebrew 安裝:

    brew install cjdns

#### OpenBSD 系統:

非常遺憾目前 OpenBSD 還在實驗階段。

    pkg_add git node gcc gmake bash

選擇 gcc-4.8.1p2 之後版本。

#### FreeBSD 系統:

所有所需相關程式已預先在 FreeBSD' ports 編譯完成。

    pkg install gmake node

#### Arch 系統:

你可以用以下指令安裝 cjdns

    pacman -S cjdns

如果你需要從原始碼重新編譯，所有需要的程式可以用以下方式安裝

    pacman -S nodejs git base-devel
    
另外，你可能會想利用 AUR 安裝包裝 `cjdns-git`。
安裝完成後，配置檔在 `/etc/cjdroute.conf`。
用以下指令啟動 `cjdns.service`：

        systemctl start cjdns

停止指令：

       systemctl stop cjdns

### 1. 從 GitHub 取得 cjdns 

從 Github 複製 Repository 並切換至下載檔案資料夾：

    git clone https://github.com/cjdelisle/cjdns.git cjdns
    cd cjdns

### 2. 初始化

    ./do

當你看見 `Build completed successfully, type ./cjdroute to begin setup.`，
再繼續以下步驟:

--------------------------------------------------------------------------------

## 安裝

執行 cjdroute 並不要顯示 HELP 選項：

    ./cjdroute

### 0. 確定你有取得東西。

    LANG=C cat /dev/net/tun

如果顯示： `cat: /dev/net/tun: File descriptor in bad state` 表示安裝成功!

如果顯示： `cat: /dev/net/tun: No such file or directory`，利用以下指令建立檔案：

    sudo mkdir -p /dev/net &&
    sudo mknod /dev/net/tun c 10 200 &&
    sudo chmod 0666 /dev/net/tun

然後再次執行 `cat /dev/net/tun` 。

如果顯示： `cat: /dev/net/tun: Permission denied` 你大概在使用一個基於
OpenVZ Virtualization Platform 的 VPS（虛擬專用伺服器）。
向你的服務供應商要求啟動 TUN/TAP Device - 這是標準作業所以他們應該要知道你的意思。

如果你用 OS X 請忽略這個步驟。


### 1. 產生一個新的配置檔（請先閱讀下方「保護好你的配置檔」部份）

    ./cjdroute --genconf >> cjdroute.conf

**保護好你的配置檔！**

失去配置檔代表你「失去密碼」以及「連接的節點」，其他與你連接的節點也會與你失聯。
配置檔被侵入代表他人可以在網路上假裝是你。

產生一個僅有你的使用者才有權限讀寫的配置檔：

    (umask 077 && ./cjdroute --genconf > cjdroute.conf)


### 2. 找一個朋友

你需要與已在一個已建立的網路內的節點才可加入已存在的網路（例如：Hyperboria）。
原因如下：

1. 因為有心人士較不會在「他人好心邀請下加入」後濫用系統。
2. 此程式目的並非要在現有網路之上並行，而是取代現有網路。
   各個連線會在未來被實體線路或無線網路取代。
3. 如果有任何異議，會有「朋友的朋友」將兩方連在一起，這使和解更容易達成
   
要找朋友，請發聲並加入我們的[社群](#community)。另外，來[Hyperboria Map][] 找尋鄰近的節點。


### 3. Connect your node to your friend's node

**To initiate the connection OUTbound**

In your conf file, you will see:

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

You can add as many connections as you want to the `connectTo` attribute,
following JSON syntax.


**To allow your friend to initiate the connection INbound**

In your conf file, you will see:

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


You need to give William Jevons (who is making the INbound connection) the following 4 items:

1. Your external IPv4
2. The port found in your conf file here:

    `// Bind to this port.
    "bind": "0.0.0.0:yourportnumberishere",`

3. Their unique password that you uncommented or created: `"password": "thisisauniquestring_002"`
4. Your public key: `"publicKey": "thisisauniqueKEY_001.k"`
5. His username: "William Jevons"

His login credentials will look something like this (with your IPv4 and port):

```javascript
"1.2.3.4:56789": {
    "login": "William Jevons",
    "password": "thisisauniquestring_002",
    "publicKey": "thisIsJustForAnExampleDoNotUseThisInYourConfFile_1.k"
}
```

Please note that you and your friend can *initiate* a
connection either outbound (from YOU --> FRIEND) or inbound (from FRIEND --> YOU)
but traffic flows both ways once the connection is established.

See [doc/configure.md](doc/configure.md) for more details on configuration,
including how to peer with other cjdns nodes over ethernet and wifi.


### 4. Secure your system - check for listening services

Once your node is running, you're now a newly minted IPv6 host. Your operating
system may automatically reconfigure network services to use this new address.
If this is not what you intend, you should check to see that you are not
offering more services then you intended to. ;)

See [doc/network-services.md](doc/network-services.md) for instructions.


### 5. Start it up!

    sudo ./cjdroute < cjdroute.conf

If you want to have your logs written to a file:

    sudo ./cjdroute < cjdroute.conf > cjdroute.log

To stop cjdns:

    sudo killall cjdroute

If you are having problems use `killall cjdroute` to return to sanity. Use
`pgrep cjdroute` or `top` to see if it running.

**NOTE!**

This starts cjdns as the root user so it can configure your system
without concern for permissions. To start cjdns as a non-root user, see
[doc/non-root-user.md](doc/non-root-user.md).


### 6. Get in IRC

Welcome to the network! You're now a network administrator. There are
responsibilities which come with being a network administrator which include
being available in case there is something wrong with your equipment. You should
stay on [IRC](#community) so that people can reach you.


## Admin interface

When cjdroute is up and running, the admin interface will be available at
`udp://localhost:11234` (this can be changed in the `cjdroute.conf`
configuration file). See [doc/admin-api.md](doc/admin-api.md) for more
information about the admin interface. There are several tools in `contrib/`
that can interact with it.

You can access the admin API with:

* the **Python library**; see [here](contrib/python/README.md).
* the **Perl library**, maintained by Mikey; see [here](contrib/perl/CJDNS/README).


## Reporting issues
1. Don't
2. Get on IRC and talk to somebody
3. What will happen is either
 * Someone feels like fixing it
 * You feel like fixing it
 * Nobody cares about it and it will be forgotten for a while and maybe someone will hit it later
 and fix it or else it will get wiped away in a refactoring
 * Nobody can fix it at the moment but it is considered worth remembering because it has great
 significance to the way the code is developed, in this case it needs to be explained in technical
 terms by someone with strong familiarity with the code. They will make a pull request to the
 docs/bugs directory.
 4. Alternatively you can report the issue on the https://github.com/hyperboria/cjdns.git repo.

### Security
Security issues should be reported on IRC the same as other bugs. We don't have a closed
group of people with special knowledge so that means the default security reporting method is
full disclosure.
see: https://github.com/cjdelisle/cjdns/blob/master/doc/security_specification.md to see if a
possible security issue is really a security issue.


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
