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
#ifndef BencSerializer_H
#define BencSerializer_H

#include "benc/Object.h"
#include "io/Writer.h"
#include "io/Reader.h"

struct BencSerializer
{
    /**
     * Serialize a string and write to a writer.
     *
     * @param writer the Writer to write to.
     * @param string the string to write.
     * @return whatever the Writer returns when writing.
     */
    int32_t (* const serializeString)(struct Writer* writer,
                                      const String* string);

    /**
     * Parse a string, reading in with the first callback and writing to the second.
     *
     * @param reader a Reader which will be asked for the data to parse. This implementation
     *               assumes the reader's pointer is alligned on the first digit of the length
     *               of the string and will leave the read pointer on the first character AFTER
     *               the end of the string.
     * @param writer a Allocator which will be used to store data.
     * @param stringPointer a pointer which will be set to the string.
     * @return 0 if everything goes well, -1 if NULL returned by the writer indicating an array
     *           overflow, -2 if -1 returned by the reader indicating an array underflow,
     *           -3 if content unparsable.
     */
    int32_t (* const parseString)(struct Reader* reader,
                                  struct Allocator* allocator,
                                  String** stringPointer);

    /**
     * Write an integer as decimal in bencoded format.
     * the integer 10 would be written as "i10e"
     *
     * @param writer the Writer to write to.
     * @param integer the number to write.
     * @return whatever the Writer returns when writing.
     */
    int32_t (* const serializeint64_t)(struct Writer* writer,
                                       int64_t integer);

    /**
     * Parse an integer, read in with the reader and set the intPointer to the value of the int.
     *
     * @param reader a Reader which will be asked for the data to parse. This implementation
     *               assumes the reader's pointer is alligned on the 'i' which begins the integer
     *               and will leave the read pointer on the first character AFTER the 'e' which
     *               ends the integer.
     * @param output a pointer to a memory location which will be set to the value of the int.
     * @return 0 if everything goes well, -2 if -1 returned by the reader indicating an
     *           array underflow, -3 if content unparsable.
     */
    int32_t (* const parseint64_t)(struct Reader* reader,
                                   int64_t* output);

    /**
     * Serialize a list.
     *
     * @param writer the Writer to write to.
     * @param List the list to serialize.
     * @return whatever the Writer returns when writing.
     */
    int32_t (* const serializeList)(struct Writer* writer,
                                    const List* list);

    /**
     * Parse a list.
     *
     * @param reader a Reader which will be asked for the data to parse. This implementation
     *               assumes the reader's pointer is alligned on the 'l' which signifies the
     *               beginning of the list. This will leave the pointer on the first character
     *               AFTER the 'e' which signifies the end of the list.
     * @param allocator a Allocator which will be used to store data.
     * @param output a pointer which will be set to the location of the List.
     * @return 0 if everything goes well, -2 if -1 returned by the reader indicating an array
     *           underflow, -3 if content unparsable.
     */
    int32_t (* const parseList)(struct Reader* reader,
                                struct Allocator* allocator,
                                List* output);

    /**
     * Serialize a dictionary.
     *
     * @param writer the Writer to write to.
     * @param dictionary the dictionary to serialize.
     * @return whatever the Writer returns when writing.
     */
    int32_t (* const serializeDictionary)(struct Writer* writer,
                                          const Dict* dictionary);

    /**
     * Parse a dictionary, reading in with the first callback and writing to the second.
     *
     * @param reader a Reader which will be asked for the data to parse. This implementation
     *               assumes the reader's pointer is alligned on the 'd' which indicates
     *               dictionary and will leave the read pointer on the first character AFTER
     *               the 'e' which ends the dictionary.
     * @param allocator a Allocator which will be used to store data.
     * @param output a dictionary pointer which will be set to the output. Dict* out = NULL;
                     is enough.
     * @return 0 if everything goes well -2 if -1 returned by read indicating an array underflow,
     *           -3 if content unparsable.
     */
    int32_t (* const parseDictionary)(struct Reader* reader,
                                      struct Allocator* allocator,
                                      Dict* output);
};

#endif
