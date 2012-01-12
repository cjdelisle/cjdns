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
#ifndef SERIALIZATIONMODULE_H
#define SERIALIZATIONMODULE_H

#include "DHTModules.h"

/**
 * Register a new SerializationModule.
 *
 * @param registry the module registry to register with.
 * @param allocator the means of aquiring memory for the serialization module.
 */
#ifdef __GNUC__
__attribute__((nonnull))
#endif
void SerializationModule_register(struct DHTModuleRegistry* registry, const struct MemAllocator* allocator);

#endif
