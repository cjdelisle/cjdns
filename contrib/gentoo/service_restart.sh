#!/bin/sh

my_email=""
send_email="false"
for svc in $(rc-status --crashed); do
	rc-service $svc -- --nodeps restart
	echo "$svc has crashed and has been restarted."
        if [ "$send_email" = "true" ]; then
          echo "$svc has crashed and has been restarted." | mail -s "$svc has been restarted." "$my_email"
        fi
done
