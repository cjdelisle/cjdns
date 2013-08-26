# cjdns

[![Build Status](https://travis-ci.org/cjdelisle/cjdns.png?branch=master)](https://travis-ci.org/cjdelisle/cjdns)

Безопасная, зашифрованная сеть для обычных людей.

Cjdns - это зашифрованная IPv6 сеть в которой используютсяпубличные ключи шифрования для присвоения публичного адреса и распределенной таблицы маршрутизации (DHT). Это позволяет создавать сети с очень простой настройкой, которые будут защищены от потенциальных проблем текущих, обычных IPv4 и IPv6 сетей.


## Зачем?

Для того, что бы дать обычным людям больше возможностей при обмене информацией.

Cjdns позволяет решить множество проблем текущих сетей, при его использовании - вы полностью обладаете своей инфраструктурой обмена информацией. 

Кроме того, данный протокол позволяет решить проблему оптимального пере-распределения трафика не делая практически ничего.

Пример - Сколько у вас дома есть открытых Wi-Fi сетей?
Наверно больше 2, а теперь представим, что они работают в Mesh режиме, динамически перенаправляя нагрузку и распределяя трафик.

Централизация власти проявляется в закрытии таких сайтов как Wikileaks, и любых других - неугодных власти.

В цивилизованном, демократическом обществе каждый должен иметь свободу слова, а цензурироватся могут лишь такие очевидно-плохие вещи как: Детская порнография, мошенечество, спам.
И конечно, решения о цензуре - должно принимать общество, а не Правительство.


## Как мы близки к финальной версии?

В данный момент, работает тестовая сеть в которой имеется от 500 до 1000 узлов.

Cjdns было протестировано на  x86, amd64, ARMv5, ARMv7, MIPS, PowerPC32 и PowerPC64. 
Тестирование на расзличных Linux системах продолжается. 

Програмное обеспечение работает стабильно, но мы не знает как поведут себя в реальном мире наши алгоритмы и протоколы.  
Пожалуйста обновляйтесь на последнюю версию cjdns, таким образом, с помощью тестирования, мы сможем определить оптимальный набор алгоритмов и протоколов для использования в реальном мире.


## Ты можешь помочь!

Нам нужны добровольцы для тестирования по автоматической сборке cjdns на Apple OS X системе, если вы можете пожертвовать нам деньги для этого дела - напишите нам на почту. 
Или же вы можете помочь нам, предоставив 24/7 shell доступ к компьютеру с системой OS X для проведения тестирования.
Всё это можно обсудить в нашем IRC канале.


## Анонимность

В отличии от TOR, I2P, и Freenet’a, cjdns планировалась не как анонимная есть. Она не использует случайную маршрутизацию, динамические маршруты и другие технологии для обеспечения анонимности. 

Наоборот, cjdns использует самый оптимальный маршрут, мы пошли на такой шаг, для обеспечения максимальной производительности.

С другой стороны, некий уровень анонимности всё же присутствует, для того что бы определить ваше физическое положение на основе вашего cjdns адреса, потребуется сделать трассировку по IPv4 адресам от конечной ноды, до вас, запросив информацию по проходящему трафику от операторов всех транзитных нод.


## Безопастность

Когда вы получаете информационный пакет из интернета, это означает, что он предназначен для вас, но никто не знает, модифицировался ли пакет по пути или же нет. 
Множество програм которые взаимодействуют с интернетом, не проверяют неизменность пакета, а такие програмно-аппаратные средства как DPI, могут вносить значительные изменения в пакеты.
Cjdns гарантирует, что пакет по пути был не кем не модифицирован, что подтверждается криптографическими средствами, которые используются в этом протоколе.

Cjdns защищает информацию которая поступает к вам, от использования заведомо ложного маршрута, что позволяет полностью избежать атаки man-in-the-middle.


## Простота

Представьте себе такую компьютерную сеть, в которой все что нужно сделать инженеру - это подключить компьтер сетевым кабелем, компьютер сам узнает как найти других и сам сконфигурируется. 
Это и есть основная цель cjdns. Конечо же, сетевые специалисты всё еще будут нужны, для решения сложных технических вопросов, но бОльшую часть решения проблем сможет взять на себя cjdns.

## Масштабируемость

С основе Cjdns лежат принципы одноранговой сети, таким образом, cjdns может мастшабироватся бесконечно - нет большой нагрузки на какой-то определенный узел.
Cjdns использует распределенную хэш таблицу (DHT) для обмена маршрутами, это позволяет не иметь карту всей сети каждому узлу (не раздувать таблицу маршрутов).

## Testimonials

    23:26 <@jercos> well, cjdns is now officially more reliable than the open internet for getting to my cheaper VPSes :|


## Как работает роутинг?

Для того, что бы понять как работает маршрутизация в cjdns, очень важно понимать, как она работает в существующем интернете.
На каждой развилке, маршрутизатор читает адрес назначения в пакете и решает куда его направить.

В cjdns сети, пакет который идет на локальный (програмный) маршрутизатор, помечается, добавляя в него наилучший маршрут для пакета. Лучший маршрут - это самый ближний физически к вам роутер, имеющий адрес близкий по значнию с адресом назначения.
Следующий роутер в цепочке так же читает данное поле и отправляет пакет в нужном направлении, кроме того, роутеры постоянно взаимодействуют друг с другом, обмениваясь информацией по маршрутам с помощью Kademlia DHT design.


## А что насчет DNS?
DNS - это сложная система, которую трудно реализовать без участия какого-либо центрального сервера. 
Cjdns сообщество еще не решило какой вектор развития пример разработка DNS в этой сети, если у вас есть идеи - обсудите их с нами в IRC.


## Почему проект называется cjdns?

Cjdns  - это совмещение двух слов cjd и DNS, но проект поменял направление своего развития практически в начале, 
и у него всё еще нет замены DNS, но имя осталось. Если вы хотите - можете придумать своб расшифровку названия


## Что еще почитать

Пожалуйста прочтите [the Whitepaper](rfcs/Whitepaper.md)

Если вам всё еще интересен проект - присоединяйтесь к обсуждению в IRC:

  * irc://irc.EFNet.org/#cjdns
  * http://chat.efnet.org:9090/?channels=%23cjdns&Login=Login

Спасибо за ваше время и интерес,
Caleb James DeLisle  ==  cjdelisle  ==  cjd

--------------------------------------------------------------------------------


# Как установить cjdns

Эти инструкции предназначены для Debian дистрибутива и аналогичных (Ubuntu) 

## 0. Устанавливаем утилиты компиляции.

    sudo apt-get install cmake git build-essential

Установка cmake желательна, но не обязательна. Если cmake не найден в процессе установки или его версия слишком старая - он будет загружен автоматически при установке.

## 1. Скачиваем cjdns из GitHub.

Клонируйте репозиторий из GitHub:

    git clone https://github.com/cjdelisle/cjdns.git cjdns
    cd cjdns

## 2. Компилируем.

    ./do

Подождите надписи `Build completed successfully, type ./cjdroute to begin setup.`, как только она появится - действуйте дальше:

--------------------------------------------------------------------------------


# Установка

Запустите cjdroute без параметров для отображения информации и доступных опций:

    ./cjdroute

## 0: Проверьте что у вас все установлено корректно.

    cat /dev/net/tun

Если ответ: `cat: /dev/net/tun: File descriptor in bad state` Отлично!

Если ответ: `cat: /dev/net/tun: No such file or directory`

Создайте его:

    sudo mkdir /dev/net ; sudo mknod /dev/net/tun c 10 200 && sudo chmod 0666 /dev/net/tun

Затем `cat /dev/net/tun` снова.

Если ответ: `cat: /dev/net/tun: Permission denied` Вы скорее всего используете виртуальный сервер (VPS) на основе технологии виртуализации OpenVZ. Попросите своего провайдера услуг включить TUN/TAP устройство, это стандартный протокол, ваш провайдер должен быть в курсе.


## 1: Генерируем новый файл с настройками.

    ./cjdroute --genconf >> cjdroute.conf

## 2: Найдите друга.

Для доступа в сеть, вам потребуется человек который уже в сети.

Найти людей которые готовы подключить вам вы можете найти в IRC канале, либо вы можете использовать данные тестового пира http://hype.rusblock.com/Public_peers.html

  * irc://irc.EFNet.org/#cjdns
  * http://chat.efnet.org:9090/?channels=%23cjdns&Login=Login


## 3: Заполните информацию о найденом друге.

В файле cjdroute.conf, вы увидите:

            // Nodes to connect to.
            "connectTo":
            {
                // Add connection credentials here to join the network
                // Ask somebody who is already connected.
            }

После добваления данных друга он будет выглядеть так:

            // Nodes to connect to.
            "connectTo":
            {
                "0.1.2.3:45678":
                {
                    "password": "thisIsNotARealConnection",
                    "publicKey": "thisIsJustForAnExampleDoNotUseThisInYourConfFile.k"
                }
            }

Вы можете добавлять столько данных для подключения к друзьям, сколько захотите.

Ваши собственные данные для подключения к ВАМ находятся в секции "authorizedPasswords" в JSON формате. Не редактируйте их - дайте их тем кто хочет к вам подключится.

Выглядит это так:

        /* These are your connection credentials
           for people connecting to you with your default password.
           adding more passwords for different users is advisable
           so that leaks can be isolated.

            "your.external.ip.goes.here:12345":
            {
                "password": "thisIsNotARealConnectionEither",
                "publicKey": "thisIsAlsoJustForAnExampleDoNotUseThisInYourConfFile.k"
            }
        */

`your.external.ip.goes.here` это поле вы должны заменить на ваш IPv4 адрес которые люди будут использовать для подключения к вам, через старый интернет.

Страница [rfcs/configure.md](rfcs/configure.md) содержит больше информации о подключении и настройке.

## 4: Безопастность - Проверьте открытые порты.

Once your node is running, you're now a newly minted IPv6 host. Your operating
system may automatically reconfigure network services to use this new address.
If this is not what you intend, you should check to see that you are not
offering more services then you intended to.  ;)

See [doc/network-services.md](doc/network-services.md) for instructions.


## 5: Start it up!

    sudo ./cjdroute < cjdroute.conf

If you want to have your logs written to a file:

    sudo ./cjdroute < cjdroute.conf > cjdroute.log

To stop cjdns:

    sudo killall cjdroute
 
If you are having problems use `killall cjdroute` to return to sanity. Use `pgrep cjdroute` or `top` to see if it running.

## 6: Get in IRC

Welcome to the network, you are now a real network administrator.
There are responsibilities which come with being a network administrator which include
being available in case there is something wrong with your equipment. You can connect to irc via
irc.efnet.org.
The channel to join is #cjdns and you should stay there so that we are able to reach you.

## Notes

This starts cjdroute as the root user so cjdroute can configure your system and shed permissions. If you really want to start cjdroute as a non-root user, see Non-Standard Setups below.

Protect your conf file! A lost conf file means you lost your password and connections and anyone who connected to you will nolonger be able to connect. A *compromised* conf file means that other people can impersonate you on the network.

    chmod 400 cjdroute.conf
    mkdir /etc/cjdns && cp ./cjdroute.conf /etc/cjdns/

# Accessing the cjdns admin interface

When cjdnroute is up and running, an administrative interface will listen on localhost:11234
(this can be changed in the cjdroute.conf configuration file).

You can access this api using the following tools, to get interesting information.

See [admin/README.md](admin/README.md) for more information about the Admin
interface.


python library
--------------

cjdns comes with a python library to access the api. For more information, read
the [readme](contrib/python/README.md).


perl library
------------

The perl port of the python api library is maintained by Mikey. For usage
instructions, head over to the [readme](contrib/perl/CJDNS/README).


# Advanced configuration

* [Run cjdns as a non-root user](doc/non-root-user.md)
* [Setup a cjdns NAT gateway for your LAN](doc/nat-gateway.md)
* [Install cjdns on OpenIndiana](doc/open-indiana.md)


[beyond pain]: https://lists.torproject.org/pipermail/tor-dev/2012-October/004063.html
