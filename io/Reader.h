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
#ifndef READER_H
#define READER_H

/* size_t */
#include <stdlib.h>

/**
 * Reader interface which reads data from a source and fails safe rather than overreading.
 */
struct Reader {
    /** The internal state of the Reader. */
    void* const context;

    /**
     * Read some content from a buffer or other source.
     *
     * @param readInto a pointer to a memory location which will have content written to it.
     * @param length the number of bytes to read. If this number is 0 then the next
     *               byte will be returned without incrementing the pointer.
     * @param thisReader the Reader which is being called. Use: reader->read(X, Y, reader);
     * @return 0 if read went well, -1 if the content ran out and no more could be read.
     */
    int (* const read)(void* readInto, size_t length, const struct Reader* thisReader);

    /**
     * Advance the pointer a number of bytes without reading any.
     * This function will happily skip off the end of the source and the next read will fail.
     *
     * @param byteCount how far to advance the pointer
     * @param thisReader the Reader which is being called. Use: reader->skip(Y, reader);
     */
    void (* const skip)(size_t byteCount, const struct Reader* thisReader);

    /**
     * @param thisReader the reader itself.
     * @return the total number of bytes which have been read OR SKIPPED by this reader.
     */
    size_t (* const bytesRead)(const struct Reader* thisReader);
};

#endif
