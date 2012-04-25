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
#ifndef CJDHTConstants_H
#define CJDHTConstants_H

#include "benc/Object.h"

// Signifying that this message is a query and defining the query type.
String* const CJDHTConstants_QUERY;

// Find nodes query. "fn"
String* const CJDHTConstants_QUERY_FN;

// Ping query
String* const CJDHTConstants_QUERY_PING;

// A search target (address)
String* const CJDHTConstants_TARGET;

// Response with nodes. "n"
String* const CJDHTConstants_NODES;

// Transaction id
String* const CJDHTConstants_TXID;

// Version which is in ping responses.
String* const CJDHTConstants_VERSION;

#endif
