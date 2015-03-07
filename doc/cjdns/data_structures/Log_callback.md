### sorry not sure what to make of this quite yet
typedef void (* Log_callback) (struct Log* log,
                               enum Log_Level logLevel,
                               const char* file,
                               int line,
                               const char* format,
                               va_list args);
