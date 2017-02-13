# Installing cjdns on raspbian jessie

This is a short guide how to setup a raspbian jessie cjdns box.

## Install packages

```bash
sudo apt install nodejs build-essential git-svn
```

## Clone, compile, install

```bash
sudo -i
cd /opt
git clone https://github.com/cjdelisle/cjdns.git
cd cjdns
NO_TEST=1 Seccomp_NO=1 ./do
ln -s /opt/cjdns/cjdroute /usr/bin
(umask 077 && ./cjdroute --genconf > /etc/cjdroute.conf)
cp contrib/systemd/cjdns.service /etc/systemd/system/
systemctl enable cjdns
systemctl start cjdns
```
