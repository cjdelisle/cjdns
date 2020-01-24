Installing CJDNS on Fedora
==========================
(last tested on Fedora 22, those with fedora versions older than 22 should substitute yum for dnf.)

## Install from dnf (easy)

```bash
sudo dnf install cjdns
```

Skip straight down to "Generating a config"

## For development

### Prerequisites
```bash
sudo dnf install git nodejs gcc
```

### Getting cjdns
```bash
git clone https://github.com/cjdelisle/cjdns
cd cjdns/
```

### Do you want to build crashey branch ?  (optional)

```bash
git checkout crashey
```

### Building cjdns
```bash
./do
```

## Generating a config
```bash
./cjdroute --genconf | sudo tee /etc/cjdroute.conf
```

## Setting cjdns to autostart on boot

```bash
sudo cp cjdns.service /etc/systemd/system/cjdns.service # This gives systemd some information about cjdns.
sudo systemctl enable cjdns.service #This sets cjdns to be started on boot. if you don't want that, feel free to leave this line out.
sudo systemctl start cjdns.service #This actually starts cjdns.
```

## Check the logs
```bash
sudo systemctl status -l cjdns
```
