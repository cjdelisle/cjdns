# Simple install

Tested on ubuntu 18.04LTS, Fedora 35+

```
git clone https://github.com/cjdelisle/cjdns.git
cd cjdns/
./do
sudo sh ./contrib/simple-install/cjdns-installer.sh
```

You can edit configuration in `/etc/cjdroute.conf` and restart cjdns service with `sudo systemctl restart cjdns.service`.

