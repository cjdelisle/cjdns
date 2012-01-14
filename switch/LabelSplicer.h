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
#ifndef LABEL_SPLICER_H
#define LABEL_SPLICER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * Splice a label and a label fragment together.
 *
 */
uint64_t LabelSplicer_splice(uint64_t goHere_be, uint64_t viaHere_be);

/**
 * Get the label for a particular destination from a given source.
 * This needs to be called before handing out a label because if a source interface is
 * represented using more bits than the destination interface, the destination interface
 * must be padded out so that the switch will find the source and destination labels compatable.
 *
 * @param target_be the label for the location to send to in big endian.
 * @param whoIsAsking_be the label for the node which we are sending the target to in big endian.
 * @return the modified target for that node in big endian.
 */
uint64_t LabelSplicer_getLabelFor(uint64_t target_be, uint64_t whoIsAsking_be);

/**
 * Determine if the node at the end of the given label is one hop away.
 *
 * @param label_be the label to test.
 * @return true if the node is 1 hop away, false otherwise.
 */
bool LabelSplicer_isOneHop(uint64_t label_be);

#endif
