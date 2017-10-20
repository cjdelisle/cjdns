# Installing cjdns on Ubuntu 16.04

This is a short guide how to setup an Ubuntu cjdns box. Make sure you are running all this command as root either by "sudo su -" or log in as root.

## Install packages

	apt-get install nodejs make gcc git python2

## Clone, compile, install

	cd /opt
	git clone https://github.com/cjdelisle/cjdns.git
	cd cjdns
	./do
	ln -s /opt/cjdns/cjdroute /usr/bin
	cp contrib/systemd/cjdns.service /etc/systemd/system/
	cp contrib/systemd/cjdns-resume.service /etc/systemd/system/
	systemctl enable cjdns
	systemctl start cjdns

