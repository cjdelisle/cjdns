#ifndef EXCEPTION_HANDLER_H
#define EXCEPTION_HANDLER_H

struct ExceptionHandler
{
    void (* exception)(char* message, int code, struct ExceptionHandler* handler);

    void* context;
};

#endif
