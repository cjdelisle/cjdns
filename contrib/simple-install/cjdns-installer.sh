#!/bin/sh
cp cjdroute /usr/bin/
cp makekeys /usr/bin/
cp privatetopublic /usr/bin/
cp publictoip6 /usr/bin/
cp randombytes /usr/bin/
cp contrib/systemd/cjdns.service /etc/systemd/system/
cp contrib/systemd/cjdns-resume.service /etc/systemd/system
systemctl enable --now cjdns.service
