Systemd Init for CJDNS - Setup Instructions

1. First, 'cjdns.conf.d' should be moved to '/etc/conf.d/cjdns' before editing the locations inside to match your system's configuration

2. You should then move 'cjdns.service.script' to '/usr/lib/systemd/scripts/cjdns' and set its permissions to executable

3. Now, if you aren't putting 'cjdns.sh' in your system's $PATH, ensure all reference contained in '/usr/lib/systemd/scripts/cjdns' include its full path

4. Finally, move 'cjdns.service' to '/usr/lib/systemd/system/cjdns.service' and run 'systemctl --system daemon-reload' before loading the service
