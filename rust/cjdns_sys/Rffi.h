#ifndef rffi_H
#define rffi_H

// This file is generated from src/rffi.rs using cbindgen

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "cffi.h"

typedef struct {
  Iface_t *internal;
  Iface_t *external;
} Rffi_IfWrapper_t;

Rffi_IfWrapper_t Rffi_testwrapper_create(Allocator_t *a);

Rffi_IfWrapper_t Rffi_android_create(Allocator_t *a);

/**
 * Replaces libuv's function:
 *
 * int uv_inet_ntop(int af, const void* src, char* dst, size_t size)
 */
int32_t Rffi_inet_ntop(bool is_ip6, const void *addr, uint8_t *dst, uint32_t dst_sz);

/**
 * Replaces libuv's function:
 *
 * int uv_inet_pton(int af, const char* src, void* dst) {
 */
int32_t Rffi_inet_pton(bool is_ip6, const char *src, uint8_t *addr);

/**
 * Non-monotonic nanosecond time, which has no relationship to any wall clock.
 */
uint64_t Rffi_hrtime(void);

/**
 * Monotonic millisecond time.
 */
uint64_t Rffi_now_ms(void);

#endif /* rffi_H */
