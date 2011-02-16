#include <stdlib.h>
#include <string.h>

#include "bencode.h"

char sampleData[] = {
    'l',
        '1', '2', ':', 'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\0',
        '1', '4', ':', 'g', 'o', 'o', 'd', 'b', 'y', 'e', ' ', 'w', 'o', 'r', 'l', 'd', '\0',
        'd',
            '2', ':', 'A', '\0',
                 'i', '1', 'e',
            '2', ':', 'B', '\0',
                 'i', '2', 'e',
            '2', ':', 'L', '\0',
                 'i', '1', '2', 'e',
            '2', ':', 'Z', '\0',
                 'i', '-', '4', 'e',
        'e',
    'e'
};

char sampleArg[] = "@sample";

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        BENC_LOG_EXCEPTION("usage: bcopy <in_file> <out_file>");
        BENC_LOG_EXCEPTION("usage: bcopy @sample <out_file>");
        return EXIT_FAILURE;
    }
    bobj_t *o;
    if (strncmp(argv[1], sampleArg, sizeof(sampleArg)))
    {
        o = bdec_file(argv[1]);
        if (NULL == o)
        {
            return EXIT_FAILURE;
        }
    }
    else
    {
        bbuf_t b;
        b.base = sampleData;
        b.len = sizeof(sampleData);
        b.ptr = b.base;
        o = bdec_mem(&b);
    }
    if (!benc_file(o, argv[2]))
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
