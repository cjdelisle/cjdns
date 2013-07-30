#!/usr/bin/env bash

# halp.sh
# ircerr@ EFNet / HypeIRC
# 20130202.v4
# "HALP! I rm'd cjdroute.conf by accident! OMG! HALP!" script.
# Recover deleted cjdroute.conf if cjdns is still running

cat << EOF

So you deleted your cjdroute.conf ay?

                      dddddddd                                                
IIIIIIIIII            d::::::d  iiii                            tttt          
I::::::::I            d::::::d i::::i                        ttt:::t          
I::::::::I            d::::::d  iiii                         t:::::t          
II::::::II            d:::::d                                t:::::t          
  I::::I      ddddddddd:::::d iiiiiii    ooooooooooo   ttttttt:::::ttttttt    
  I::::I    dd::::::::::::::d i:::::i  oo:::::::::::oo t:::::::::::::::::t    
  I::::I   d::::::::::::::::d  i::::i o:::::::::::::::ot:::::::::::::::::t    
  I::::I  d:::::::ddddd:::::d  i::::i o:::::ooooo:::::otttttt:::::::tttttt    
  I::::I  d::::::d    d:::::d  i::::i o::::o     o::::o      t:::::t          
  I::::I  d:::::d     d:::::d  i::::i o::::o     o::::o      t:::::t          
  I::::I  d:::::d     d:::::d  i::::i o::::o     o::::o      t:::::t          
  I::::I  d:::::d     d:::::d  i::::i o::::o     o::::o      t:::::t    tttttt
II::::::IId::::::ddddd::::::ddi::::::io:::::ooooo:::::o      t::::::tttt:::::t
I::::::::I d:::::::::::::::::di::::::io:::::::::::::::o      tt::::::::::::::t
I::::::::I  d:::::::::ddd::::di::::::i oo:::::::::::oo         tt:::::::::::tt
IIIIIIIIII   ddddddddd   dddddiiiiiiii   ooooooooooo             ttttttttttt  

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
    echo "Now don't let that happen again."
  fi
done
