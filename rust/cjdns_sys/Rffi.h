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

#endif /* rffi_H */
