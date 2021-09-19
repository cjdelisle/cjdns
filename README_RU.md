# cjdns

[English](README.md)
[Hrvatski](README_HR.md)
[Svenska](README_SV.md)
[Ελληνικά](README_GR.md)
[Deutsch](README_DE.md)
[繁體中文](README_ZHT.md)
[Español](README_ES.md)
[Français](README_FR.md)
[Português brasileiro](README_PT-BR.md)

Безопасная, зашифрованная сеть для обычных людей.

Cjdns — это зашифрованная IPv6 сеть, в которой используются публичные ключи шифрования для присвоения публичного адреса и распределённой таблицы маршрутизации (DHT). Это позволяет создавать сети с очень простой настройкой, которые будут защищены от потенциальных проблем ныне существующих IPv4 и IPv6 сетей.

[![Build Status](https://api.travis-ci.org/cjdelisle/cjdns.png?branch=master)](https://travis-ci.org/cjdelisle/cjdns)
[![For next commit](https://tip4commit.com/projects/941.svg)](https://tip4commit.com/github/cjdelisle/cjdns)

## Рекомендательные письма

    23:26 <@jercos> well, cjdns is now officially more reliable than the open
                    internet for getting to my cheaper VPSes :|

    12:52 < mariner> so i don't know if it's been done before, and i assume it's
                     obvious, but I think it's neat. Currently on hype from an
                     airplane


## Как работает маршрутизация?

Чтобы понять как работает маршрутизация в cjdns, очень важно понимать, как она работает в существующем интернете. На каждой развилке маршрутизатор читает адрес назначения в пакете и решает, куда его направить.

В cjdns сети на каждом пакете, который идет на локальный (программный) маршрутизатор, отмечается наилучший маршрут для него. Лучший маршрут — это самый близкий (физически) к вам роутер, имеющий адрес, близкий по значению к адресу назначения.

Следующий роутер в цепочке также читает данное поле и отправляет пакет в нужном направлении. Кроме того, роутеры постоянно взаимодействуют друг с другом, обмениваясь информацией по маршрутам с помощью Kademlia DHT.


## А что насчёт DNS?

DNS — это сложная система, которую трудно реализовать без участия какого-либо центрального сервера.

Сообщество cjdns ещё не решило, какой вектор развития примет разработка DNS в этой сети; если у вас есть идеи — то, мы вас ждём в IRC.


## Почему проект называется cjdns?

Cjdns — это совмещение двух слов cjd и DNS, но проект поменял направление своего развития практически в начале, и у него всё еще нет замены DNS, но имя осталось. Можете придумать свою расшифровку названия, если хотите.


Спасибо за ваше время и интерес,
Caleb James DeLisle  ==  cjdelisle  ==  cjd

## Community

* irc://irc.efnet.org/#cjdns_ru ([web client][IRC Web])
* [Hyperboria][]
* [/r/darknetplan][]
* [#cjdns on Twitter][]


## Documentation

* [Project Goals](doc/projectGoals_ru.md)(RU)
* [Cjdns Whitepaper](doc/Whitepaper.md)
* [Cjdns on Wikipedia][]

Advanced configuration:

* [Setup a cjdns NAT gateway for your LAN](doc/nat-gateway.md)
* [Install cjdns on OpenIndiana](doc/open-indiana.md)

### License

[Available here](LICENSE)

Thank you for your time and interest,

The cjdns developers.


--------------------------------------------------------------------------------


# Как установить cjdns

Эти инструкции предназначены для дистрибутива Debian и аналогичных (Ubuntu, Mint).

## 0. Устанавливаем утилиты компиляции.

Установка  [Node.js](https://nodejs.org/) желательна, но не
обязательна. Если Node.js не найден в процессе установки или его
версия слишком старая — он будет загружен автоматически при установке.

#### Дистрибутивы основанные на Debian:

    sudo apt-get install nodejs git build-essential python2.7

#### Дистрибутивы основанные на Fedora 22+:

    sudo dnf install nodejs git
    sudo dnf install @development-tools

#### Дистрибутивы основанные на RHEL (добавляет репозиторий EPEL):

    sudo yum localinstall https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
    sudo yum install nodejs git
    sudo yum install @development-tools

#### Сборка из пакета:

    sudo yum localinstall https://kojipkgs.fedoraproject.org//packages/cjdns/17.4/4.el6/src/cjdns-17.4-4.el6.src.rpm

Если вы используете ноутбук и вводите его в спящий режим, то cjdroute понадобится
несколько минут, чтобы приготовить кофе и понять, что произошло. Вы можете значительно
ускорить это следующим способом:

    systemctl enable cjdns-resume

Сервис возобновления перезапускает cjdns, если система выходит из спячки.

#### Gentoo

    emerge --ask nodejs sys-devel/gcc dev-lang/python:3.4 dev-vcs/git

#### macOS:

Установка с [Homebrew](https://brew.sh/):

    brew install cjdns

Установка с [MacPorts](https://www.macports.org/):

    sudo port install cjdns

#### OpenBSD:

    pkg_add git node gcc gmake bash

Выберете версию gcc-4.8.1p2 или новее.

#### FreeBSD:

Всё что вам необходимо, доступно в коллекции портов FreeBSD.

    pkg install gmake node

#### Arch:

Вы можете установить cjdns выполнив следующее
    pacman -S cjdns

Если вы хотите скомпилировать самостоятельно, то всё что вам необходимо может быть установлено следующей командой

    pacman -S nodejs git base-devel

Как альтернативный вариант, вы можете установить через AUR из пакета `cjdns-git`.

После установки файл конфигурации находится тут `/etc/cjdroute.conf`.
Для запуска сервиса `cjdns.service`, выполните:

    systemctl start cjdns

Для остановки:

    systemctl stop cjdns

#### Gentoo:

cjdns еще не находится в главном репозитории Gentoo, поэтому вам придется использовать наложение.
Самый простой способ - это использовать Layman, но вы можете сделать это и вручную.

##### Layman:

Во-первых, вам нужно установить layman.

    emerge layman

Если layman установлен правильно, то вы можете добавить наложение

    layman -f
    layman -a weuxel

Для будущего обновления использования наложения

    layman -S

Теперь вы можете установить cjdns

    emerge cjdns

##### Ручками:

Вам придется клонировать репозиторий наложения

    cd /opt
    git clone https://github.com/Weuxel/portage-weuxel.git

Теперь скажите portage использовать этот репозиторий

    cd /etc/portage/repos.conf/

Создайте файл `portage-weuxel.conf` с текстом:

    [weuxel]
    location = /opt/portage-weuxel
    masters = gentoo
    auto-sync = yes

Теперь синхронизируйте

    emerge --sync

И установите cjdns

    emerge cjdns

#### Автоматическое обнаружение сбоев и перезагрузка.

Скопируйте скрипт инициализации openrc из `contrib/openrc` в `/etc/init.d/`, и модифицируйте параметры `CONFFILE` и `command` в соответствии с вашими требованиями.

Затем запустите выполнив:

    /etc/init.d/cjdns start

Настройте систему init для автоматического запуска

    rc-update add cjdns default

Скопируйте скрипт service_restart `contrib/gentoo/service_restart.sh` в любой удобный каталог в 
вашей системе, и измените адрес электронной почты. Если вы не хотите получать уведомления, то закомментируйте всю строку.
Теперь добавьте запись в crontab, наподобие этой

    # Restart crashed Services
    * * * * *       root	/path/to/script/service_restart.sh

#### Solus:

Зависимости:

    sudo eopkg install nodejs git build-essential system.devel python gcc binutils kernal-headers xorg-server-devel

Затем следуйте шагам ниже.

## 1. Скачиваем cjdns из GitHub.

Склонируйте репозиторий из GitHub:

    git clone https://github.com/cjdelisle/cjdns.git cjdns
    cd cjdns

## 2. Компилируем.

    ./do

Дождитесь сообщения `Build completed successfully, type ./cjdroute to begin setup.`, и как только оно появится — действуйте дальше:

--------------------------------------------------------------------------------


# Установка

Запустите cjdroute без параметров для отображения информации и доступных опций:

    ./cjdroute

## 0: Убедитесь, что у вас всё установлено корректно.

    LANG=C cat /dev/net/tun

Если ответ: `cat: /dev/net/tun: File descriptor in bad state`,то всё отлично!

Если ответ: `cat: /dev/net/tun: No such file or directory`,то просто создайте его:

    sudo mkdir /dev/net ; sudo mknod /dev/net/tun c 10 200 && sudo chmod 0666 /dev/net/tun

Затем снова `cat /dev/net/tun`.

Если ответ: `cat: /dev/net/tun: Permission denied`, вы скорее всего используете виртуальный сервер (VPS) на основе технологии виртуализации OpenVZ. Попросите своего провайдера услуг включить TUN/TAP устройство, это стандартный протокол, ваш провайдер должен быть в курсе.


## 1: Генерируем новый файл с настройками.

    ./cjdroute --genconf >> cjdroute.conf

## 2: Найдите пира.

Для доступа в сеть вам потребуется человек который уже в сети.

Найти людей, которые готовы подключить вас, вы можете найти в IRC канале, либо можете использовать данные тестового пира http://hype.rusblock.com/Public_peers.html

### Англоязычный канал:

  * irc://irc.EFNet.org/#cjdns
  * http://chat.efnet.org:9090/?channels=%23cjdns&Login=Login

### Русскоязычный канал:

  * irc://irc.EFNet.org/#cjdns-ru
  * http://chat.efnet.org:9090/?channels=%23cjdns-ru&Login=Login

## 3: Заполните информацию о найденном пире.

В файле cjdroute.conf вы увидите:

            // Nodes to connect to.
            "connectTo":
            {
                // Add connection credentials here to join the network
                // Ask somebody who is already connected.
            }

После добавления данных пира он будет выглядеть как-то так:

            // Nodes to connect to.
            "connectTo":
            {
                "0.1.2.3:45678":
                {
                    "login": "user-login",
                    "password": "thisIsNotARealConnection",
                    "publicKey": "thisIsJustForAnExampleDoNotUseThisInYourConfFile.k"
                }
            }

Вы можете добавлять столько данных для подключения к пирам, сколько захотите.

Ваши собственные данные для подключения _к вам_ находятся в секции "authorizedPasswords" в JSON формате. Не редактируйте их — дайте их тем, кто хочет к вам подключится.

Выглядит это так:

        /* These are your connection credentials
           for people connecting to you with your default password.
           adding more passwords for different users is advisable
           so that leaks can be isolated.

            "your.external.ip.goes.here:12345":
            {
                "login": "user-login",
                "password": "thisIsNotARealConnectionEither",
                "publicKey": "thisIsAlsoJustForAnExampleDoNotUseThisInYourConfFile.k"
            }
        */

`your.external.ip.goes.here` нужно заменить на ваш IPv4 адрес, который люди будут использовать для подключения к вам через интернет.

Страница [rfcs/configure.md](rfcs/configure.md) содержит больше информации о подключении и настройке.

## 4: Безопасность — Проверьте открытые порты.

Как только вы подключились к глобальной сети cjdns — вы стали обладателем публичного IPv6 адреса. Убедитесь, что запущенные на вашей машине сервисы недоступны из внешней сети.

Подробнее: [doc/network-services.md](doc/network-services.md)


## 5: Запускаем!

    sudo ./cjdroute < cjdroute.conf

Если вы хотите записывать логи в файл, то:

    sudo ./cjdroute < cjdroute.conf > cjdroute.log

Остановка cjdns осуществляется следующим образом:

    sudo killall cjdroute

Если вы испытываете проблемы при использовании `killall cjdroute`. Используйте `pgrep cjdroute` или `top` для определения, запущен ли он.

## 6: Войдите в IRC

Добро пожаловать в сеть, в которой вы настоящий системный администратор.
По любым вопросам вы можете проконсультироваться в IRC: канал `#cjdns` на сервере `irc.efnet.org` или используйте канал `#cjdns-ru` который на русском языке и на том же сервере.


## Заметки

Нормальная установка cjdns подразумевает запуск от имени администратора. Если вы не считаете нужным давать такие привилегии cjdns, то вы можете воспользоваться инструкцией ниже, запустив его не от root.

Никому не передавайте свой файл настроек: именно он гарантирует вашу безопасность в сети и хранит ваш идентификатор. Если у вас его украдут, то ваша безопасность будет скомпрометирована, и злоумышленник сможет представиться вами в сети.

    chmod 400 cjdroute.conf
    mkdir /etc/cjdns && cp ./cjdroute.conf /etc/cjdns/


# Получаем доступ к интерфейсу администратора в cjdns

Когда cjdroute запущен и работает, интерфейс администратора доступен по адресу localhost:11234 (обратите внимание, что это не веб-интерфейс, веб-интерфейс вы можете получить используя nodejs cjdns admin). Настройки порта и пароля для доступа к интерфейсу администратора доступны в файле cjdroute.conf.

Каким образом получить доступ к данному интерфейсу, вы можете прочитать по этим ссылкам: [doc/admin-api.md](doc/admin-api.md).


Библиотека для python
---------------------

Cjdns поставляется с библиотекой для python, которая позволяет получить доступ к admin API. Больше информации вы можете получить здесь: [readme](contrib/python/README.md).


Библиотека для perl
-------------------

Библиотека для perl поддерживается разработчиком Mikey. Инструкции по использованию можно получить тут: [readme](contrib/perl/CJDNS/README).

# Отправка отчёта об ошибках

1. Не отправляйте отчёт в этот репозиторий, вместо этого отправляйте его на https://github.com/hyperboria/bugs/issues
2. Расскажите об этом кому-нибудь в IRC
3. Произойдёт что-то из этого:
 * Кто-то почувствует в себе силы исправить это
 * Вы почувствуете в себе силы исправить это
 * Никого не заинтересовала эта ошибка, и она будет забыта на какое-то время, и, может быть, кто-то исправит её позже, либо, в противном случае, ошибка исчезнет во время рефакторинга
 * В настоящий момент никто не может исправить эту ошибку, но она заслуживает внимания, так как может сильно воспрепятствовать разработке в будущем, и в этом случае ошибка должна быть технически детально описана человеком, хорошо знакомым с кодом. Он отправит запрос в директорию docs/bugs.

## Безопасность

Об ошибках безопасности следует сообщать в IRC так же, как и о других ошибках. У нас нет закрытой группы людей со специальными знаниями, поэтому по умолчанию все отчёты об ошибках безопасности являются полностью открытыми. 
Проверить, является ли потенциальная проблема безопасности действительно проблемой безопасности можно по следующей ссылке: https://github.com/cjdelisle/cjdns/blob/master/doc/security_specification.md
 
# Продвинутая настройка

* [Запуск cjdns не от root](doc/non-root-user_ru.md)
* [Настройка ПК как cjdns NAT gateway для вашей домашней сети](doc/nat-gateway.md)
* [Установка cjdns на OpenIndiana](doc/open-indiana.md)

Снова это время года... Время для проектов с открытым исходным кодом!
[![Hacktoberfest](https://img.shields.io/badge/Open%20Source-Hacktoberfest-orange.svg)](https://hacktoberfest.digitalocean.com/)

[beyond pain]: https://lists.torproject.org/pipermail/tor-dev/2012-October/004063.html
[IRC Web]: http://chat.efnet.org/irc.cgi?chan=%23cjdns_ru
[Hyperboria]: https://hyperboria.net
[/r/darknetplan]: https://www.reddit.com/r/darknetplan
[#cjdns on Twitter]: https://twitter.com/hashtag/cjdns
[Buildbots]: https://buildbot.meshwith.me/cjdns/waterfall

[Cjdns on Wikipedia]: https://ru.wikipedia.org/wiki/Cjdns
[Distributed Hash Table]: https://ru.wikipedia.org/wiki/Distributed_hash_table
[Beyond Pain]: https://lists.torproject.org/pipermail/tor-dev/2012-October/004063.html
[Kademlia]: https://ru.wikipedia.org/wiki/Kademlia

[Tor]: https://www.torproject.org
[I2P]: https://geti2p.net/en/
[Freenet]: https://freenetproject.org
