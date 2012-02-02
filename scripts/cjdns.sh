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

#
# cjdns.sh
# copy this file into your cjdns user's hime directory and add
# */5 * * * * /home/cjdns/cjdns.sh check
# to your cron tab for the cjdns user.
#

cd
dir="`pwd`/cjdns/build"

stop()
{
    kill `cat cjdroute.pid 2>>/dev/null` 2>>/dev/null
    rm cjdroute.pid
}

start()
{
    if [ -f cjdroute.log ]; then
        tar -cjf "cjdroute.log.`date +%s`.tar.bz2" cjdroute.log
    fi

    $dir/cjdroute < cjdroute.conf 2>&1 > cjdroute.log &

    sleep 1

    if [ ! -f cjdroute.pid ]; then
        echo 'Can''t find pid file, please edit cjdroute.conf and make sure'
        echo 'it is configured to save a pid file called cjdroute.pid'
        echo 'Stopping.'
        kill $!
        exit 1
    fi
}

if [ "$1" == "start" ]; then
    start();
    exit 0
fi

case "$1" in 
"restart" )
    stop();
    start();
    ;;

"stop" )
    stop();
    ;;

"check" )
    if [ -f cjdroute.pid ]; then
        # router is stopped, let's not defeat the user.

    elif ! kill -0 `cat cjdroute.pid 2>>/dev/null` 2>>/dev/null; then
        start();
    fi
    ;;
esac
