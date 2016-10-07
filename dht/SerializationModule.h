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
#ifndef SerializationModule_H
#define SerializationModule_H

#ifdef SUBNODE
    #error "this file should not be included in subnode"
#endif

#include "dht/DHTModuleRegistry.h"
#include "util/Linker.h"
Linker_require("dht/SerializationModule.c");

/**
 * Register a new SerializationModule.
 *
 * @param registry the module registry to register with.
 * @param logger
 * @param allocator the means of aquiring memory for the serialization module.
 */
void SerializationModule_register(struct DHTModuleRegistry* registry,
                                  struct Log* logger,
                                  struct Allocator* allocator);

#endif
