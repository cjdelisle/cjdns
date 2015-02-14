/* vim: set expandtab ts=4 sw=4: */
/*
 * You may redistribute this program and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef Event_H
#define Event_H

enum Event
{
    Event_SEARCH,

    /**
     * Acknoledges the search has begun and a SEARCH_END is guaranteed to be sent later.
     * contains a SearchHeader.
     */
    Event_SEARCH_BEGIN,

    /** End of search, contains a SearchHeader. */
    Event_SEARCH_END,

    Event_DISCOVERY,

    /** Must be the last entry, never emitted. */
    Event_INVALID
};

struct Event_SearchHeader
{
    enum Event event_be;

    uint8_t ipv6[16];
};

struct Event_DiscoveryHeader
{
    enum Event event_be;

    uint8_t ip6[16];

    uint8_t publicKey[32];

    uint64_t path_be;

    /** Quality of path represented by switch label (0:best ffffffff:worst) */
    uint32_t metric_be;

    uint32_t version_be;
};

#endif
