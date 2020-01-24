
# cjdns

[English](README.md)
[Русская версия](README_RU.md)
[Hrvatski](README_HR.md)
[Svenska](README_SV.md)
[Ελληνικά](README_GR.md)
[Deutsch](README_DE.md)
[Español](README_ES.md)
[Français](README_FR.md)

#### *重朔整個網路*

Cjdns 利用「加密的IPv6」及「公鑰加密」來分配網路地址並利用「Distributed Hash Table」逕行路由。它能提供近似「零配置網路（Zero-Configuration Networking）」，並且能防範在現有網路中存在的很多和安全、可擴展性相關的問題。

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

感謝你的時間和關注

cjdns 開發人員 敬上

--------------------------------------------------------------------------------

## 如何安裝 cjdns

這些說明雖然是在基於 Debian 的 Linux 發行版系統與 macOS 系統上的，但是已經為在其它發行版作業系統安裝提供了足夠多的信息，只是不要期望可以一字不差的執行指令。

### 0. 安裝相關程式

在兩種作業系統上安裝 [Node.js](http://nodejs.org/)，非必要但強烈建議。
如無 Node.js 或版本錯誤，安裝程式會自動下載安裝至源代碼資料夾中。

#### Debian 系統:

    sudo apt-get install nodejs git build-essential python2.7

#### Fedora 22+ 系統:

    sudo dnf install install nodejs git
    sudo dnf install @development-tools

#### RHEL 系統 (會新增 EPEL 的 Repository):

    sudo yum localinstall https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
    sudo yum install install nodejs git
    sudo yum install @development-tools

#### macOS 系統:

利用 [Homebrew](https://brew.sh/) 安裝:

    brew install cjdns

利用 [MacPorts](https://www.macports.org/) 安裝:

    sudo port install cjdns

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

不帶參數執行 cjdroute 時會顯示幫助信息：

    ./cjdroute

### 0. 確定你已經準備好必要的資料。

    LANG=C cat /dev/net/tun

如果顯示： `cat: /dev/net/tun: File descriptor in bad state` 表示安裝成功!

如果顯示： `cat: /dev/net/tun: No such file or directory`，利用以下指令建立檔案：

    sudo mkdir -p /dev/net &&
    sudo mknod /dev/net/tun c 10 200 &&
    sudo chmod 0666 /dev/net/tun

然後再次執行 `cat /dev/net/tun` 。

如果顯示： `cat: /dev/net/tun: Permission denied` 你大概在使用一個基於 OpenVZ Virtualization Platform 的 VPS（虛擬專用伺服器）。
向你的服務供應商要求啟用 TUN/TAP 設備 - 這是標準協議，所以他們應當明白你的意思。

如果你用 macOS 請忽略這個步驟。


### 1. 產生一個新的配置檔（請先閱讀下方「保護好你的配置檔」部份）

    ./cjdroute --genconf >> cjdroute.conf

**保護好你的配置檔！**

失去配置檔代表你丟失密碼以及已建立的連結，其他以你做為接入點的節點也會無法與你連線。
配置檔被侵入代表他人可以在網路上假裝是你。

產生一個僅有你的使用者才有權限讀寫的配置檔：

    (umask 077 && ./cjdroute --genconf > cjdroute.conf)


### 2. 找一個朋友

為了加入一個已經存在的網路（例如：Hyperboria），你需要與一個已經在該網路中的節點連接。
原因如下：

1. 因為有心人士較不會在「他人好心邀請下加入」後濫用系統。
2. 此程式目的並非要在現有網路之上並行，而是取代現有網路。各個連線會在未來被實體線路或無線網路取代。
3. 如果有任何異議，會有「朋友的朋友」將兩方連在一起，這使和解更容易達成
   
要找朋友，請發聲並加入我們的 [社群](#社群)。另外，來[Hyperboria Map][] 找尋鄰近的節點。


### 3. 連接你與朋友的節點

**由你啟始連線（向外）**

在你的配置檔，你可以找到：

``` javascript
// Nodes to connect to.
"connectTo":
{
    // Add connection credentials here to join the network
    // Ask somebody who is already connected.
}
```

一個包含許多朋友節點（向外）的配置檔類似這樣：

``` javascript
// Nodes to connect to.
"connectTo":
{
    //朋友_1 (IPv4: 0.1.2.3; IPv6 fcaa:5bac:66e4:713:cb00:e446:c317:fc39)
    "0.1.2.3:45678":
    {
        "login": "k.alexander"
        "password": "這不是真的連線_1",
        "publicKey": "這只是範例請不要真的在配置檔中用這串字_1.k"
    }

    //朋友_2 (IPv4: 5.1.2.3; IPv6 fcbb:5bac:66e4:713:cb00:e446:c317:fc39)
    "5.1.2.3:5678":
    {
        "login": "k.alexander"
        "password": "這不是真的連線_2",
        "publicKey": "這只是範例請不要真的在配置檔中用這串字_2.k"
    }
}
```

你可以依照 JSON 語法在 `connectTo` 屬性中加入無限個連線。


**讓你的朋友啟始連線（向內）**

在配置檔中，你可以找到：

``` javascript
"authorizedPasswords":
[
    // A unique string which is known to the client and server. 
    {"password": "獨一無二的密碼（請不要輸入中文）_001"}

    // More passwords should look like this. 
    // {"password": "獨一無二的密碼（請不要輸入中文）_002"}
    // {"password": "獨一無二的密碼（請不要輸入中文）_003"}
    // {"password": "獨一無二的密碼（請不要輸入中文）_004"}
    ...

    // "你的.對外.IP.位址:45678":{"password": "獨一無二的密碼（請不要輸入中文）_001","publicKey":獨一無二的金鑰_001.k"}

],
```

一個有許多朋友節點的配置檔，向內連線應該長這樣：
``` javascript
"authorizedPasswords":
[
    // A unique string which is known to the client and server. 在此輸入讓他人對你連線的密碼
    {"password": "獨一無二的密碼（請不要輸入中文）_001", "user": "k.alexander"}

    // More passwords should look like this. 其他請依此格式類推
    //William Jevons (IPv4: 0.1.2.3; IPv6 fcaa:5bac:66e4:713:cb00:e446:c317:fc39)
    {"password": "獨一無二的密碼（請不要輸入中文）_002", "user": "William Jevons"}
    //Marilyn Patel (IPv4: 5.1.2.3; IPv6 fcbb:5bac:66e4:713:cb00:e446:c317:fc39)
    {"password": "獨一無二的密碼（請不要輸入中文）_003", "user": "Marilyn Patel"}
    // {"password": "獨一無二的密碼（請不要輸入中文）_004"}
    ...

    // "你的.對外.IP.位址:45678":{"password": "獨一無二的密碼（請不要輸入中文）_001","publicKey":獨一無二的金鑰_001.k"}
],
```


你必須給 William Jevons (對你而言是向內連線) 下方四項資訊：

1. 你的外部 IPv4
2. 你可以在配置檔找到的埠號：

    `// Bind to this port.
    "bind": "0.0.0.0:你的埠號在這裡",`

3. 為你朋友設定或取消註解的唯一密碼： `"password": "獨一無二的密碼（請不要輸入中文）_002"`
4. 你的公鑰： `"publicKey": "獨一無二的金鑰_001.k"`
5. 他的登入名稱： "William Jevons"

他的配置檔中會看起來像這樣(包含你的 IPv4 和 埠號):

```javascript
"1.2.3.4:56789": {
    "login": "William Jevons",
    "password": "獨一無二的密碼（請不要輸入中文）_002",
    "publicKey": "這只是範例請不要真的在配置檔中用這串字_1.k"
}
```

請記得你與你朋友可以啟始「向外（你 --> 朋友）」或「向內（朋友 --> 你）」連線，但當通道建立後兩方皆可使用。

更進階的配置請閱讀 [doc/configure.md](doc/configure.md)，包括如何透過乙太網路與Wifi連線。

### 4. 保護你的系統 - 檢查正在使用網路的程式

當你的節點開始運行，你的電腦便成為一個新的 IPv6 伺服。你的作業系統可能會自動重新配置網路程式來使用這個新的網路位址。
如果你並不希望這麼做，你應確認你並沒有提供一些你不打算公開的服務。 ;)

更多資料請查閱 [doc/network-services.md](doc/network-services.md)。


### 5. 開始使用！

    sudo ./cjdroute < cjdroute.conf

如果你希望將紀錄存成一個檔案：

    sudo ./cjdroute < cjdroute.conf > cjdroute.log

停止 cjdns:

    sudo killall cjdroute

如果出現問題用 `killall cjdroute` 指令來結束一切。
用 `pgrep cjdroute` 或 `top` 指令來確認是否正在執行。

**注意！**

這將會讓 cjdns 以 Root 權限執行並設定你的系統，避免要求權限等提示。 請參閱[doc/non-root-user.md](doc/non-root-user.md) 來以非 Root 權限執行。


### 6. 加入 IRC

歡迎來到這個網絡！ 你現在是一個網管了。 這代表你有些成一個網管的責任，包括當你的設備出狀況時必須能回應。你應當在 [IRC](#community) 保持在線，這樣其他人才能與你聯繫。


## 管理者界面

當 cjdroute 正在執行時，你可以在 `udp://localhost:11234` 看到管理者界面 (這可以在 `cjdroute.conf` 更改). 這個檔案 [doc/admin-api.md](doc/admin-api.md) 有更多有關管理者界面的資訊。一些在 `contrib/` 的工具可以配合使用。

你可以透過以下方式使用 Admin API ：

* **Python library**; 按 [這裡](contrib/python/README.md).
* **Perl library**, 由 Mikey 維護; 按 [這裡](contrib/perl/CJDNS/README).


## 回報問題
1. 請不要在此回報問題，有問題請到 https://github.com/hyperboria/bugs/issues 回報
2. 在 IRC 上講
3. 以下是可能發生的
 * 有人主動來解決問題
 * 你主動來解決問題
 * 根本沒人理這問題然後被遺忘，也許某天有人會遇到同樣的問題然後在某次的重構中解決
 * 目前無解但是應該被記住，因為這會影響未來程式開發，這種情況必須被對程式碼有深入理解的人進行解釋。他會提出對 Bug 或文件進行修改

### 安全
安全問題應該同樣與其他 Bug 一樣在 IRC 回報。 我們並沒有一個擁有特殊技術的封閉團隊，所以預設的安全問題回報是完全公開。
請參閱： https://github.com/cjdelisle/cjdns/blob/master/doc/security_specification.md 來確認可能安全問題是真的安全問題。


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
