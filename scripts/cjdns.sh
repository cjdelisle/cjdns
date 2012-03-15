#!/bin/bash
# You may redistribute this program and/or modify it under the terms of
# the GNU General Public License as published by the Free Software Foundation,
# either version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

###
# cjdns.sh
# copy this file into your cjdns user's hime directory and add
# */5 * * * * /home/cjdns/cjdns.sh check >>/dev/null 2>>/dev/null
# to your cron tab for the cjdns user.
#
# When you type:
#  ./cjdns.sh start
# cjdns will be started and if it should stop for any reason (including the computer beign halted)
# the cron job will restart it.
# To stop it and prevent it from restarting, use:
#  ./cjdns.sh stop
# After doing an update, use:
#  ./cjdns.sh restart
# to stop it and bring it back online immedietly.
##

# path of cjdns
CJDPATH="`dirname $0`/"

# path to the cjdroute process
CJDROUTE="${CJDPATH}cjdroute"

# path to the configuration
CONF="${CJDPATH}cjdroute.conf"

# path ot the log file.
LOGTO="${CJDPATH}cjdroute.log"

# location of pid file.
PIDFILE="`${CJDROUTE} --pidfile < $CONF`"

stop()
{
    if [ -f $PIDFILE ] && kill -0 `cat $PIDFILE` > /dev/null; then
        kill `cat $PIDFILE`
        rm $PIDFILE
    fi
}

noPid()
{
    echo 'Can''t find pid file, please edit cjdroute.conf and make sure'
    echo 'it is configured to save a pid file. You can get this behavior by adding:'
    echo '    "pidFile": "/path/to/your/pid.file",'
    echo 'to your configuration.'
    echo 'Stopping.'
    exit 1
}

start()
{
    $CJDROUTE < $CONF 2>&1 >> $LOGTO &

    sleep 1

    if [ ! -f $PIDFILE ]; then
        noPid
    fi
}

if [ "$PIDFILE" == "" ]; then
    noPid
fi

case "$1" in
"start" )
    start
    ;;

"restart" )
    stop
    start
    ;;

"stop" )
    stop
    ;;

"check" )
    if [ ! -f $PIDFILE ] || ! kill -0 `cat $PIDFILE` > /dev/null; then
        start
    fi
    ;;
esac
