#!/bin/sh
cp cjdroute /usr/bin/
cp target/release/makekeys /usr/bin/
cp target/release/mkpasswd /usr/bin/
cp target/release/privatetopublic /usr/bin/
cp target/release/publictoip6 /usr/bin/
cp target/release/randombytes /usr/bin/
cp target/release/sybilsim /usr/bin/
cp contrib/systemd/cjdns.service /etc/systemd/system/
cp contrib/systemd/cjdns-resume.service /etc/systemd/system
systemctl enable --now cjdns.service
