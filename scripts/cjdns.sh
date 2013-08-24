#!/usr/bin/env bash
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

if [ -f /etc/default/cjdns ]; then
  . /etc/default/cjdns
fi

# path to the cjdns source tree, no trailing slash
if [ -z "$CJDPATH" ]; then CJDPATH=`dirname $0`; fi

# full path to the cjdroute binary
if [ -z "$CJDROUTE" ]; then CJDROUTE="${CJDPATH}/cjdns/cjdroute"; fi

# full path to the configuration file
if [ -z "$CONF" ]; then CONF="${CJDPATH}/cjdroute.conf"; fi

# path to the log file.
if [ -z "$LOGTO" ]; then LOGTO="/dev/null"; fi

load_pid()
{
    PID=$(pgrep -d " " -f "$CJDROUTE")
}

load_pid

stop()
{
    if [ -z "$PID" ]; then
        echo "cjdns is not running"
        return 1
    else
        kill $PID &> /dev/null
        while [ -n "$(pgrep -d " " -f "$CJDROUTE")" ]; do
            echo "* Waiting for cjdns to shut down..."
            sleep 1;
        done
        if [ $? -gt 0 ]; then return 1; fi
    fi
}

start()
{
    if [ -z "$PID" ]; then
        $CJDROUTE < $CONF &>> $LOGTO
        if [ $? -gt 0 ]; then
            echo "Failed to start cjdns"
            return 1
        fi
    else
        echo "cjdns is already running"
        return 1
    fi
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

update()
{
    if [ -d ${CJDPATH}/cjdns/.git ]; then
        cd ${CJDPATH}/cjdns
        git pull
        ./do || echo "Failed to update!" && exit 1
        echo "* Update complete, restarting cjdns"
        stop
        load_pid
        start
    else
        echo "The cjdns source directory does not exist"
        return 1
    fi
}

case "$1" in
    "start" )
        start
        ;;
    "restart" )
        stop
        load_pid
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
    "update" )
        update
        ;;
    *)
        echo "usage: $0 {start|stop|restart|check|update}"
esac
