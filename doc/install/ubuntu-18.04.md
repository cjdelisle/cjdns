# Installing cjdns on Ubuntu 16.04

This is a short guide how to setup an Ubuntu cjdns box.

## Install packages

	sudo apt-get install nodejs git build-essential python2.7 make

## Clone, compile, install

	cd /opt
	git clone https://github.com/cjdelisle/cjdns.git
	cd cjdns
	./do
	ln -s /opt/cjdns/cjdroute /usr/bin/
	(umask 077 && ./cjdroute --genconf > /etc/cjdroute.conf)
	cp contrib/systemd/cjdns.service contrib/systemd/cjdns-resume.service /lib/systemd/system/
	systemctl enable cjdns
	systemctl start cjdns

