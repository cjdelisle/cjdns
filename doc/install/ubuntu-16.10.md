# Installing cjdns on Ubuntu 16.04

This is a short guide how to setup an Ubuntu cjdns box. Make sure you are running all this command as root either by "sudo su -" or log in as root.

## Install packages

	apt-get install nodejs make gcc git python

## Clone, compile, install

	cd /opt
	git clone https://github.com/bonsaimesh/bonsaimesh.git
	cd bonsaimesh
	./do
	ln -s /opt/cjdns/bonsai /usr/bin
	cp contrib/systemd/bonsai.service /etc/systemd/system/
	cp contrib/systemd/bonsai-resume.service /etc/systemd/system/
	systemctl enable bonsai
	systemctl start bonsai
