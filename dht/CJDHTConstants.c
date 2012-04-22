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
#include "dht/CJDHTConstants.h"

// Signifying that this message is a query and defining the query type.
String* const CJDHTConstants_QUERY =      &(String) { .len = 1, .bytes = "q"    };

// Find nodes query. "fn"
String* const CJDHTConstants_QUERY_FN =   &(String) { .len = 2, .bytes = "fn"   };

// Ping query
String* const CJDHTConstants_QUERY_PING = &(String) { .len = 2, .bytes = "pn"   };

// A search target (address)
String* const CJDHTConstants_TARGET =     &(String) { .len = 3, .bytes = "tar"  };

// Response with nodes. "n"
String* const CJDHTConstants_NODES =      &(String) { .len = 1, .bytes = "n"    };

// Transaction id
String* const CJDHTConstants_TXID =       &(String) { .len = 4, .bytes = "txid" };

// Version which is in ping responses.
String* const CJDHTConstants_VERSION =    &(String) { .len = 1, .bytes = "v" };
