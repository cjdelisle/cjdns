#!/bin/sh -e
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
#
# Simple example IPv6 Firewall configuration.
# Derived from http://www.exp-networks.be/blog/ipv6-firewall/
#
# permits only outbound, connected and core ICMP messages on tun0
# - does not filter other interfaces at all.
# - Adds one rule to the INPUT chain
# - Adds a new chain for tun0
# - edit and change the INPUT_PORTS rules to run services.
# - there is no stop facility - if you can't figure out how to reset an
#   ip6tables firewall, you shouldn't do so!
#

# Error if ip6tables is not in the path.
which ip6tables

# Inbound TCP ports
TCP_INPUT_PORTS=""

# Inbound UDP ports
UDP_INPUT_PORTS=""

# Allowed ICMP messages
ALLOWED_ICMP="\
echo-request \
echo-reply \
"

# There is no 'assert a chain exists.
ip6tables -N CJD || ip6tables -F CJD
# Link the new table into the master INPUT table.
ip6tables -C INPUT -i tun0 -j CJD || ip6tables -I INPUT -i tun0 -j CJD

# Allow related and established connection.
ip6tables -A CJD -m state --state RELATED,ESTABLISHED -j ACCEPT

# Allow ICMP as defined in ALLOWED_ICMP
if [ -n "$ALLOWED_ICMP" ] ; then
 for ICMP_TYPE in $ALLOWED_ICMP; do
  ip6tables -A CJD -p icmpv6 --icmpv6-type ${ICMP_TYPE} -j ACCEPT
 done
fi

# Open allowed TCP ports if any
if [ -n "$TCP_INPUT_PORTS" ] ; then
 for PORT in $TCP_INPUT_PORTS; do
  ip6tables -A CJD -m state --state NEW -p tcp --dport ${PORT} \
  -j ACCEPT
 done
fi

# Open allowed UDP ports if any
if [ -n "$UDP_INPUT_PORTS" ] ; then
 for PORT in $UDP_INPUT_PORTS; do
  ip6tables -A CJD -m state --state NEW -p udp --dport ${PORT} \
  -j ACCEPT
 done
fi

# Deny all other traffic on tun0
ip6tables -A CJD -j LOG
ip6tables -A CJD -j DROP
