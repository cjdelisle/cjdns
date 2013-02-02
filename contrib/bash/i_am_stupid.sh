#!/bin/bash

# halp.sh
# ircerr@ EFNet / HypeIRC
# 20130202.v4
# "HALP! I rm'd cjdroute.conf by accident! OMG! HALP!" script.
# Recover deleted cjdroute.conf if cjdns is still running

cat <<EOF
IIIII DDDDD   IIIII  OOOOO  TTTTTTT 
 III  DD  DD   III  OO   OO   TTT   
 III  DD   DD  III  OO   OO   TTT   
 III  DD   DD  III  OO   OO   TTT   
IIIII DDDDDD  IIIII  OOOO0    TTT   
EOF

if [ "`whoami`" != "root" ]
then
  echo "-ROOT REQUIRED. run sudo $0"
  exit 1
fi

PIDS="`pidof cjdns`"
if [ "$PIDS" == "" ]
then
  echo "no cjdns procs running. Dispair! All hope is lost. Unable to help. RIP cjdroute.conf"
  exit 1
fi

for PID in $PIDS
do
  echo "-Checking $PID"
  V6="`cat /proc/$PID/fd/0 2>>/dev/null|grep 'ipv6'|cut -d\\\" -f4|head -n1`"
  if [ "$V6"  != "" ]
  then
    TS="`date +%s`"
    echo "-Saving $V6 from PID:$PID to cjdroute.conf-saved-$TS-$PID"
    cat /proc/$PID/fd/0 2>>/dev/null > cjdroute.conf-saved-$TS-$PID
  fi
done
