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
#ifndef Writer_H
#define Writer_H

#include <stdint.h>

/**
 * Writer interface which writes data to a destination and fails safe rather than overflowing.
 */
struct Writer {
    /**
     * Write some content from a buffer or other source.
     *
     * @param w the Writer which is being called.
     * @param toWrite a pointer to a memory location where content will be sourced from.
     * @param length the number of bytes to write.
     * @return 0 if write went well, -1 if there is no more space to write.
     *           if a write fails then all subsequent writes will fail with the same error
     *           so writing a large piece of content then a small footer does not require
     *           checking if the content wrote correctly before writing the footer.
     */
    int (* const write)(struct Writer* w, const void* toWrite, unsigned long length);

    uint64_t bytesWritten;
};

#define Writer_writeGeneric(bytes) \
    static inline int Writer_write##bytes (struct Writer* writer, uint##bytes##_t number) \
    {                                                                                     \
        uint##bytes##_t num = number;                                                     \
        return writer->write(writer, &num, bytes/8);                                      \
    }

Writer_writeGeneric(8)
Writer_writeGeneric(16)
Writer_writeGeneric(32)
Writer_writeGeneric(64)

#define Writer_write(writer, bytes, amount) \
    ((writer)->write((writer), (bytes), (amount)))

#define Writer_bytesWritten(writer) \
    ((writer)->bytesWritten + 0)

#endif
