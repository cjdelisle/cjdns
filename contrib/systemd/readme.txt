Systemd Init for CJDNS - Setup Instructions

1. Copy 'cjdns.service' to '/usr/lib/systemd/system/cjdns.service'

2. Run 'systemctl --system daemon-reload'

3. Ensure your cjdroute.conf is located @ '/etc/cjdroute.conf'

4. Enable the service to load on boot by running 'systemctl enable cjdns'

5. Start the service by running 'systemctl start cjdns'
