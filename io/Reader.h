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
     * @param this the Reader which is being called. Use: reader->read(X, Y, reader);
     * @return 0 if read went well, -1 if the content ran out and no more could be read.
     */
    int (* const read)(void* readInto, size_t length, struct Reader* this);

    /**
     * Advance the pointer a number of bytes without reading any.
     * This function will happily skip off the end of the source and the next read will fail.
     *
     * @param byteCount how far to advance the pointer
     * @param this the Reader which is being called. Use: reader->skip(Y, reader);
     */
    void (* const skip)(size_t byteCount, struct Reader* this);
};

#endif
