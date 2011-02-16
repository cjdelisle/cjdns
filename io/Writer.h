#ifndef WRITER_H
#define WRITER_H

/* size_t */
#include <stdlib.h>
/* uint64_t */
#include <stdint.h>

/**
 * Writer interface which writes data to a destination and fails safe rather than overflowing.
 */
struct Writer {
    /** The internal state of the Writer. */
    void* const context;

    /**
     * Write some content from a buffer or other source.
     *
     * @param toWrite a pointer to a memory location where content will be sourced from.
     * @param length the number of bytes to write.
     * @param this the Writer which is being called. Use: writer->write(X, Y, writer);
     * @return 0 if write went well, -1 if there is no more space to write.
     *           if a write fails then all subsequent writes will fail with the same error
     *           so writing a large piece of content then a small footer does not require
     *           checking if the content wrote correctly before writing the footer.
     */
    int (* const write)(void* toWrite, size_t length, struct Writer* this);

    uint64_t (* const bytesWritten)(struct Writer* writer);
};

#endif
