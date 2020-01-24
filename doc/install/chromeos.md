# Installing CJDNS on ChromeOS

This is a short guide how to setup CJDNS on a Chromebook.

## Install packages

Enable the Linux beta and open a terminal.

```bash
sudo apt install nodejs build-essential git-svn
```

## Clone, compile, install

```bash
sudo -i

# Build cjdns
cd /opt
git clone https://github.com/cjdelisle/cjdns.git
cd cjdns
./do
ln -s /opt/cjdns/cjdroute /usr/bin

# Generate a config file
(umask 077 && ./cjdroute --genconf > /etc/cjdroute.conf)
# Regarding cjdns' configuration you can continue reading here:
# https://github.com/cjdelisle/cjdns#2-find-a-friend

# Set up a system service that runs on startup
cp contrib/systemd/cjdns.service /etc/systemd/system/
systemctl enable cjdns
# the service `cjdns-resume` does not work currently
systemctl start cjdns
```
