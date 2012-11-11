#!/bin/sh
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

CJDPATH="${0##*/}"

CJDROUTE="$CJDPATH/cjdns/cjdroute"

CONF="$CJDPATH/cjdroute.conf"

LOGTO="/dev/null"

do_start() {
    "$CJDROUTE" <"$CONF" >>"$LOGPATH" 2>&1 &
}

do_stop() {
    pkill -x cjdns
}

is_running() {
    pgrep -x cjdns >/dev/null
}

case $1 in
start)
    do_start
    ;;
stop)
    do_stop
    ;;
restart)
    do_stop && do_start
    ;;
status)
    is_running
    ;;
check)
    is_running || start
    ;;
esac
