# Installing cjdns on debian wheezy

This is a short guide how to setup a debian wheezy cjdns box.

## Enable backports

	echo "deb http://http.debian.net/debian wheezy-backports main" >> /etc/apt/sources.list

## Install systemd

	aptitude install systemd-sysv

## Install packages

	aptitude install nodejs build-essential git

## Clone, compile, install

	cd /opt
	git clone https://github.com/cjdelisle/cjdns.git
	cd cjdns
	./do
	ln -s /opt/cjdns/cjdroute /usr/bin
	./cjdroute --genconf > /etc/cjdroute.conf
	cp contrib/systemd/cjdns.service /etc/systemd/system/
	systemctl enable cjdns

