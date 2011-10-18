#ifndef ENDIAN_H
#define ENDIAN_H

#include <stdint.h>

#if defined(__BYTE_ORDER) && defined(__BIG_ENDIAN)
    #if (__BYTE_ORDER == __BIG_ENDIAN)
        #define Endian_isBigEndian() 1
    #else
        #define Endian_isBigEndian() 0
    #endif
#else
    static uint32_t Endian_isBigEndian()
    {
        union {
            uint32_t i;
            char c[4];
        } bint = {0x01020304};

        return bint.c[0] == 1; 
    }
#endif

#endif
