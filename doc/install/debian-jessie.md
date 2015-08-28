# Installing cjdns on debian jessie

This is a short guide how to setup a debian jessie cjdns box.

## Install packages

	apt install nodejs build-essential git

## Clone, compile, install

	cd /opt
	git clone https://github.com/cjdelisle/cjdns.git
	cd cjdns
	./do
	ln -s /opt/cjdns/cjdroute /usr/bin
	(umask 077 && ./cjdroute --genconf > /etc/cjdroute.conf)
	cp contrib/systemd/cjdns.service /etc/systemd/system/
	systemctl enable cjdns
	systemctl start cjdns

