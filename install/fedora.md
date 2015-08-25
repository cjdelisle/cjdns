Installing CJDNS on Fedora
==========================
(last tested on Fedora 22, those with fedora versions older than 22 should substitute yum for dnf.)

#Prerequisites
```bash
sudo dnf install git automake nodejs libseccomp-devel gcc 
```

#Getting cjdns
```bash
git clone https://github.com/hyperboria/cjdns
```

#Building cjdns
```bash
cd cjdns/
./do
```

#Generating a config
```bash
./cjdroute --genconf > cjdroute.conf
```

#Setting cjdns to autostart on boot.

First you'll want to edit contrib/systemd/cjdns.service to properly reflect where your cjdns binary and configuration are.
Then, run these commands:

```bash
sudo cp cjdns.service /etc/systemd/system/cjdns.service # This gives systemd some information about cjdns.
sudo systemctl enable cjdns.service #This sets cjdns to be started on boot. if you don't want that, feel free to leave this line out.
sudo systemctl start cjdns.service #This actually starts cjdns.
```

Checking the logs:
```bash
sudo systemctl status -l cjdns
```
