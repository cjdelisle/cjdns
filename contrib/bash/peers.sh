#!/usr/bin/env bash

set -ex

# Never again restart cjdroute for new peerings!
#
# Eases peering by updating both cjdroute.conf and the running cjdroute. Also
# produces a JSON config fragment to give to your new peer.
#
# Usage:
#   cd cjdns/
#   contrib/bash/peers.sh jane jane@example.net <jane's ipv6>
#
# Requires:
# - $HOME/.cjdnsadmin
# - writable cjdroute.conf
#
# TODO:
# - unwrap the handout object, so that it can actually be copy-pasted
#

HANDOUT=/opt/handout.json
CJDROUTECONF=/etc/cjdroute.conf
CJDNS_DIR=/opt/cjdns

if [ ! -f $HANDOUT ]; then
  echo "Please copy $CJDNS_DIR/contrib/bash/handout.json to $HANDOUT and edit it according to your preference."
  exit 1
fi

cd $CJDNS_DIR
git checkout `cat REVISION`

user=$1
contact=$2
ipv6=$3
password=`./makekeys | head -c32`
publicKey=`cat $CJDROUTECONF | cjdroute --cleanconf | jq -r '.publicKey'`
localIpv6=`./publictoip6 $publicKey`

# add password to running cjdroute process, using admin api
response=`./contrib/python/cexec -p "AuthorizedPasswords_add(\"$password\", \"$user\", 1, \"$ipv6\")"`
echo $response | grep 'none' > /dev/null
if [ $? -ne 0 ]; then echo "admin error: $response" ; exit 1; fi
echo "Added [$user] to the currently running cjdroute"

# write password to config file
cp $CJDROUTECONF $CJDROUTECONF.backup
cat $CJDROUTECONF.backup | cjdroute --cleanconf | jq ".authorizedPasswords |= . + [{user: \"$user\", contact: \"$contact\", ipv6: \"$ipv6\", password: \"$password\"}]" > $CJDROUTECONF
echo "Added [$user] to $CJDROUTECONF"

# print config snippet to hand out to the peer
echo "Config snippet to hand out:"
cat $HANDOUT | jq ".[].ipv6 = \"$localIpv6\"" | jq ".[].publicKey = \"$publicKey\"" | jq ".[].password = \"$password\""
