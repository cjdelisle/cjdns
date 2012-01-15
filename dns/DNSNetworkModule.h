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
#include <event2/event.h>
#include <memory/Allocator.h>

/**
 * Create a new DNS network module.
 *
 * @param base a LibEvent context.
 * @param socket the socket to bind to.
 * @param allocator the memory allocator to use for getting memory.
 */
struct DNSModule* DNSNetworkModule_new(struct event_base* base,
                                       evutil_socket_t socket,
                                       struct Allocator* allocator);

/**
 * Register the module.
 *
 * @param module the module to register.
 * @param registry the DNSModuleRegistry to call the on an incoming message.
 * @return whatever is returned by DNSModules_Register().
 */
int DNSNetworkModule_register(struct DNSModule* module,
                              struct DNSModuleRegistry* registry);
