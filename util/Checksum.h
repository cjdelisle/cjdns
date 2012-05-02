/* vim: set expandtab ts=4 sw=4: */

#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <inttypes.h>
#include <stddef.h>

uint32_t generateChecksum(unsigned char *buf, size_t nbytes, uint32_t sum);

#endif

