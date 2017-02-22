# Installing cjdns on raspbian jessie

This is a short guide how to setup a raspbian jessie cjdns box.

## Install packages

```bash
sudo apt install nodejs build-essential git-svn
```

## Clone, compile, install

```bash
sudo -i

# Upgrade Node the official way
# https://nodejs.org/en/download/package-manager/#debian-and-ubuntu-based-linux-distributions
curl -sL https://deb.nodesource.com/setup_7.x | sudo -E bash -
apt install -y nodejs

# Build cjdns
cd /opt
git clone https://github.com/cjdelisle/cjdns.git
cd cjdns
NO_TEST=1 Seccomp_NO=1 ./do
ln -s /opt/cjdns/cjdroute /usr/bin

# Generate a config file
(umask 077 && ./cjdroute --genconf > /etc/cjdroute.conf)
# Regarding cjdns' configuration you can continue reading here:
# https://github.com/cjdelisle/cjdns#2-find-a-friend

# Set up a system service that runs on startup
cp contrib/systemd/cjdns.service /etc/systemd/system/
systemctl enable cjdns
systemctl start cjdns
```
