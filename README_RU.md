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

Imagine a network where all the engineer has to do was plug in the wires, the computers
instinctively know how to find each other. This is the ultimate goal of cjdns. It is understood
that some network engineering will always be necessary but reducing it and simplifying what remains
are top priorities.


## Scalability

Cjdns is built around a bold and unproven assumption. It is assumed that a non-hierarchical network
can scale. Cjdns uses a technique similar to Distributed Hash Tables in order to spread the load of
routing among a number of nodes rather than requiring every node to know the full path to every
other node.


## Testimonials

    23:26 <@jercos> well, cjdns is now officially more reliable than the open internet for getting to my cheaper VPSes :|


## How does routing work?

In order to understand how cjdns routing works, it is important to understand
how the existing Internet works when you send a packet. At each "intersection
in the road" the router reads the address on the packet and decides which turn
it should take.

In the cjdns net, a packet goes to a router and the router labels the packet
with directions to the router best able to handle it. That is, a router which
is physically nearby and has an address numerically close to the destination
address of the packet. The directions are added to the packet to allow it to go
through a number of routers with minimal handling,
*a verifiable form of source routing*. They just read the label and bounce the
packet wherever the next bits in the label tell them to. Routers have a
responsibility to "keep in touch" with other routers that are physically close
by and numerically near to their address.

The router engine is a modified implementation of the Kademlia DHT design.


## What about DNS?

DNS is a complex system to implement and highly complex to implement without
central authority.  If you would like to offer help with this part, I invite
you to come join.


## Where did the name cjdns come from?

Cjdns was based on a codebase which was originally intended to handle name resolution (DNS) and so
it was a combination of 'cjd' and 'dns'. The project changed direction early on and currently is
still lacking DNS resolution but the name stuck. Make up your own acronym for it if you like.


## Further Reading & Discussion

Please read [the Whitepaper](rfcs/Whitepaper.md), or at least skim it.

If you are still interested in this project and want to follow it,
get in the channel on IRC:

  * irc://irc.EFNet.org/#cjdns
  * http://chat.efnet.org:9090/?channels=%23cjdns&Login=Login

Thank you for your time and interest,
Caleb James DeLisle  ==  cjdelisle  ==  cjd

--------------------------------------------------------------------------------


# How to install cjdns

These instructions are for Debian / Debian derived distributions. They should
be informative enough for user on other distributions - just don't expect them
to work verbatim.

## 0. Install the build tools you will need.

    sudo apt-get install cmake git build-essential

Installing cmake, although preferable, is not strictly necessary.  If cmake is
unavailable or an unacceptable version, it will be downloaded and built in the
source tree.

## 1. Retrieve cjdns from GitHub.

Clone the repository from GitHub and change to the source directory:

    git clone https://github.com/cjdelisle/cjdns.git cjdns
    cd cjdns

## 2. Build.

    ./do

Look for `Build completed successfully, type ./cjdroute to begin setup.`, then proceed below:

--------------------------------------------------------------------------------


# Setup

Run cjdroute without options for HELP:

    ./cjdroute

## 0: Make sure you've got the stuff.

    cat /dev/net/tun

If it says: `cat: /dev/net/tun: File descriptor in bad state` Good!

If it says: `cat: /dev/net/tun: No such file or directory`

Create it using:

    sudo mkdir /dev/net ; sudo mknod /dev/net/tun c 10 200 && sudo chmod 0666 /dev/net/tun

Then `cat /dev/net/tun` again.

If it says: `cat: /dev/net/tun: Permission denied` You're probably using a VPS based on the OpenVZ virtualization platform. Ask your provider to enable the TUN/TAP device, this is standard protocol so they should know exactly what you need.


## 1: Generate a new configuration file.

    ./cjdroute --genconf >> cjdroute.conf

## 2: Find a friend.

In order to get into the network you need to meet someone who is also in the network and connect to them. This is required for a number of reasons:

1. It is a preventative measure against abuse because bad people will be less likely to abuse a system after they were, in an act of human kindness, given access to that system.
2. This is not intended to overlay The Old Internet, it is intended to replace it. Each connection will in due time be replaced by a wire, a fiber optic cable, or a wireless network connection.
3. In any case of a disagreement, there will be a "chain of friends" linking the people involved so there will already be a basis for coming to a resolution.

tl;dr Get out and make some human contact once in a while!

You can meet people to peer with in the IRC channel:

  * irc://irc.EFNet.org/#cjdns
  * http://chat.efnet.org:9090/?channels=%23cjdns&Login=Login

NOTE: If you're just interested in setting up a local network between your own computers, this step is not necessary.

## 3: Fill in your friend's info.

In your cjdroute.conf file, you will see:

            // Nodes to connect to.
            "connectTo":
            {
                // Add connection credentials here to join the network
                // Ask somebody who is already connected.
            }

After adding their connection credentials, it will look like:

            // Nodes to connect to.
            "connectTo":
            {
                "0.1.2.3:45678":
                {
                    "password": "thisIsNotARealConnection",
                    "publicKey": "thisIsJustForAnExampleDoNotUseThisInYourConfFile.k"
                }
            }

You can add as many connections as you want to your "connectTo" section.

Your own connection credentials will be shown in a JSON comment above in your
"authorizedPasswords" section. Do not modify this but if you want to allow someone
to connect to you, give it to them.

It looks like this:

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

`your.external.ip.goes.here` is to be replaced with the IPv4 address which people will use to
connect to you from over The Old Internet.

The [rfcs/configure.md](rfcs/configure.md) page contains more details on
configuration, including how to peer with other cjdns nodes over ethernet
(including wifi).

## 4: SECURITY RISK - Check for listening services.

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
