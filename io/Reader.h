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
#ifndef Reader_H
#define Reader_H

#include <stdint.h>

/**
 * Reader interface which reads data from a source and fails safe rather than overreading.
 */
struct Reader {
    /**
     * Read some content from a buffer or other source.
     *
     * @param thisReader the Reader which is being called.
     * @param readInto a pointer to a memory location which will have content written to it.
     * @param length the number of bytes to read. If this number is 0 then the next
     *               byte will be returned without incrementing the pointer.
     * @return 0 if read went well, -1 if the content ran out and no more could be read.
     */
    int (* const read)(struct Reader* thisReader, void* readInto, unsigned long length);

    /**
     * Advance the pointer a number of bytes without reading any.
     * This function will happily skip off the end of the source and the next read will fail.
     *
     * @param thisReader the Reader which is being called.
     * @param byteCount how far to advance the pointer.
     */
    void (* const skip)(struct Reader* thisReader, unsigned long byteCount);

    /** The total number of bytes which have been read OR SKIPPED by this reader. */
    uint64_t bytesRead;
};


#define Reader_readGeneric(bytes) \
    static inline uint##bytes##_t Reader_read##bytes (struct Reader* reader) \
    {                                                                        \
        uint##bytes##_t num;                                                 \
        reader->read(reader, &num, bytes/8);                                 \
        return num;                                                          \
    }

Reader_readGeneric(8)
Reader_readGeneric(16)
Reader_readGeneric(32)
Reader_readGeneric(64)


#define Reader_read(reader, readInto, bytes) \
    (reader)->read((reader), (readInto), (bytes))

#define Reader_skip(reader, bytes) \
    (reader)->skip((reader), (bytes))

#define Reader_bytesRead(reader) \
    ((reader)->bytesRead + 0)

#endif
