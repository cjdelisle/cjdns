#ifndef ABORT_HANDLER_H
#define ABORT_HANDLER_H

#include "exception/ExceptionHandler.h"

#include <stdio.h>

/** Internal callback, please use AbortHandler_INSTANCE instead. */
static void AbortHandler_callback(char* message, int code, struct ExceptionHandler* handler)
{
    handler = handler;
    fprintf(stderr, "Error: %s (code: %d)", message, code);
    abort();
}

/**
 * The exception handler.
 * Prints the message to stderr and aborts the program.
 */
struct ExceptionHandler* AbortHandler_INSTANCE = &(struct ExceptionHandler) {
    .exception = AbortHandler_callback
};

#endif
