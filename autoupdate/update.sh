#!/bin/bash

# Set this to the developer who signs tags which you trust.
trustedKey='0F89CF79E785DCD0BCF6964843771307596E2918'
trustedKeyShortName='596E2918'

# Keyservers which will be used to check for revocation of above key.
keyServers=(
    pgp.mit.edu
    esperanza.ubuntu.com
    gozer.rediris.es
    kerckhoffs.surfnet.nl
    keys.kfwebs.net
    keys.nayr.net
    keys.niif.hu
    keyserver.oeg.com.au
    keyserver.stack.nl
    lorien.prato.linux.it
    pgp.zdv.uni-mainz.de
    pgpkeys.logintas.ch
    pgpkeys.pca.dfn.de
    stinkfoot.org
);
keyServer=${keyServers[$((RANDOM %= $((${#keyServers[@]}))))]}

# Repositories from which to try pulling.
repos=(
    git://github.com/cjdelisle/cjdns.git
);
repo=${repos[$((RANDOM %= $((${#repos[@]}))))]}

workDir="`cd;pwd`/cjdns"

#-----------------------------------------------------------------------------
#   Code
#-----------------------------------------------------------------------------

checkBuildDir()
{
    if [ ! -e "build" ]; then
        echo 'No build directory, please build and setup configuration files first.'
        exit 1
    fi
}

checkIsUp()
{
    #Run It
    #gdb ./cjdroute -ex "run < ./cjdroute.conf"
    checkBuildDir
    cd $workDir/build
    PIDS="`ps axw|grep -v 'grep\|tail'|grep cjdroute|cut -b -6`"
    if [ "$PIDS" == "" ]
    then
        mv cat cjdroute.log >>cjdroute.log.`date +%s` 2>>/dev/null
        echo -n >cjdroute.log
        echo "-Restarting" | tee -a cjdroute.log
        echo >>cjdroute.log
        ./cjdroute < cjdroute.conf 1>> cjdroute.log 2>> cjdroute.log &
    else
        echo "cjdroute is running on PID:$PIDS"
    fi
    git checkout master
}

# Check proper user.
if [ "`whoami`" != "cjdns" ]; then
    echo "run $0 as user [cjdns] not [`whoami`]"
    exit 1
fi

cd $workDir

# Check for updates.
if [ "`git pull $repo master 2>/dev/null | grep 'Already up-to-date.'`" != "" ]; then
    checkIsUp
    exit 0
fi

revlist="`git rev-list --tags --max-count=1`"
version="`git describe --tags $revlist`"

if [ "$version" == "`cat /tmp/cjdnsLastVersion 2>/dev/null`" ]; then
    checkIsUp
    exit 0
fi

keyServer=${keyServers[$((RANDOM %= $((${#keyServers[@]}))))]}
while [ "`gpg --recv-keys --keyserver $keyServer $trustedKey | grep 'not found on keyserver'`" != "" ]
do
    keyServer=${keyServers[$((RANDOM %= $((${#keyServers[@]}))))]}
done

if [ "`git tag -v $version 2>&1 | grep 'gpg: Good signature from '`" == "" ]; then
    echo "Couldn't find good signature."
    exit 1
fi
if [ "`git tag -v $version 2>&1 | grep 'This key has been revoked by its owner!'`" != "" ]; then
    echo "Key revoked"
    exit 1
fi
if [ "`git tag -v $version 2>&1 | grep "using DSA key ID $trustedKeyShortName"`" == "" ]; then
    echo "Wrong key"
    exit 1
fi

echo $version > /tmp/cjdnsLastVersion || exit 1
git checkout $version || exit 1

cd $workDir || exit 1

checkBuildDir
cd build
if [ ! -e "cjdroute.conf" ]; then
    echo 'No configuration file (called cjdroute.conf) type cjdroute for instructions.'
    exit 1
fi

cmake .. && make

#Check VER
MD5_NOW="`md5sum cjdroute|cut -d\  -f1`"
MD5_OLD="`cat cjdns.md5 2>>/dev/null`"

if [ "$MD5_NOW" != "$MD5_OLD" ]
then
  echo "-MD5 Changed $MD5_OLD -> $MD5_NOW"
  echo "$MD5_NOW" >cjdns.md5
  PIDS="`ps axw|grep -v grep|grep cjdroute|cut -b -6`"
  for PID in $PIDS
  do
      PS="`ps -p $PID 2>>/dev/null|grep cjdroute`"
      if [ "$PS" != "" ]
      then
          echo "Killing $PID ($PS)"
          kill -9 $PID
      fi
  done
#  killall -9 cjdroute 2>>/dev/null
fi

checkIsUp
