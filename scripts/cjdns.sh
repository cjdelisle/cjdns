#!/bin/bash
#
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
#
# Copy this script to the desired location, then add the following to the cjdns user's crontab
# */5 * * * * /path/to/cjdns.sh check >>/dev/null 2>>/dev/null
#
# Start cjdns if it isn't already running (and set the above cronjob to restart failed processes):
#   ./cjdns.sh start
#
# Stop cjdns if it's currently running (and set the above cronjob not to restart failed processes):
#  ./cjdns.sh stop
#
# Check whether cjdns is currently running:
#  ./cjdns.sh status
#
# Restart cjdns after upgrades and changes to the config:
#  ./cjdns.sh restart
##

# path of cjdns
if [ -z "$CJDPATH" ]; then CJDPATH="`dirname $0`/"; fi

# path to the cjdroute process
if [ -z "$CJDROUTE" ]; then CJDROUTE="${CJDPATH}cjdns/cjdroute"; fi

# path of the cjdns process
if [ -z "$CJDNS" ]; then CJDNS="${CJDPATH}cjdns/cjdns"; fi

# path to the configuration
if [ -z "$CONF" ]; then CONF="${CJDPATH}cjdroute.conf"; fi

# path ot the log file.
if [ -z "$LOGTO" ]; then LOGTO="/dev/null"; fi

PID=$(pgrep -d " " -f "$CJDNS")

stop()
{
    [ ! -z "$PID" ] && kill $PID &> /dev/null
    if [ $? -gt 0 ]; then return 1; fi
}

start()
{
    $CJDROUTE < $CONF 2>&1 >> $LOGTO &
    if [ $? -gt 0 ]; then return 1; fi
}

status()
{
    echo -n "* cjdns is "
    if [ -z "$PID" ]; then
        echo "not running"
        exit 1
    else
        echo "running"
        exit 0
    fi
}

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
    "status" )
        status
        ;;
    "check" )
        ps aux | grep -v 'grep' | grep 'cjdns core' > /dev/null 2>/dev/null || start
        ;;
    *)
        echo "usage: /etc/rc.d/cjdns {start|stop|restart|check}"
esac
