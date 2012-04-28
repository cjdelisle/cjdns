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

#define BSTR_SIZEOF(x) &(String) { .bytes = x, .len = sizeof(x) - 1 }

// Signifying that this message is a query and defining the query type.
static String* const CJDHTConstants_QUERY = String_CONST_SO("q");

// Find nodes query. "fn"
static String* const CJDHTConstants_QUERY_FN = String_CONST_SO("fn");

// Ping query
static String* const CJDHTConstants_QUERY_PING = String_CONST_SO("pn");

// A search target (address)
static String* const CJDHTConstants_TARGET = String_CONST_SO("tar");

// Response with nodes. "n"
static String* const CJDHTConstants_NODES = String_CONST_SO("n");

// Transaction id
static String* const CJDHTConstants_TXID = String_CONST_SO("txid");

// Version which is in ping responses.
static String* const CJDHTConstants_VERSION = String_CONST_SO("v");

#endif
