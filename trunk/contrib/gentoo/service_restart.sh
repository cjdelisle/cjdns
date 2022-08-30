#!/bin/bash

for svc in $(rc-status --crashed); do
	rc-service $svc -- --nodeps restart
	echo "$svc has crashed and has been restarted." | mail -s "$svc has been restarted." <your_email>
done
