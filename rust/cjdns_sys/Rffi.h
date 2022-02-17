#ifndef rffi_H
#define rffi_H

// This file is generated from src/rffi.rs using cbindgen

#include "RffiPrefix.h"

typedef struct RTypes_CryptoAuth2_t RTypes_CryptoAuth2_t;

/**
 * A data repository, that groups objects related to this event loop.
 */
typedef struct Rffi_EventLoop Rffi_EventLoop;

/**
 * The guard of an acquired [`GCL`].
 */
typedef struct Rffi_Glock_guard Rffi_Glock_guard;

/**
 * The handle returned to C, used to talk to the timer task.
 */
typedef struct Rffi_TimerTx Rffi_TimerTx;

typedef struct {
  uint8_t octets[16];
  uint8_t netmask[16];
  bool is_ipv6;
} Rffi_Address;

typedef struct {
  const char *name;
  uint8_t phys_addr[6];
  bool is_internal;
  Rffi_Address address;
} Rffi_NetworkInterface;

extern const uintptr_t Rffi_CURRENT_PROTOCOL;

int Rffi_CryptoAuth2_addUser_ipv6(String_t *password,
                                  String_t *login,
                                  uint8_t *ipv6,
                                  RTypes_CryptoAuth2_t *ca);

int Rffi_CryptoAuth2_removeUsers(RTypes_CryptoAuth2_t *context, String_t *user);

RTypes_StrList_t *Rffi_CryptoAuth2_getUsers(const RTypes_CryptoAuth2_t *ca, Allocator_t *alloc);

RTypes_CryptoAuth2_t *Rffi_CryptoAuth2_new(Allocator_t *allocator,
                                           const uint8_t *privateKey,
                                           Random_t *random);

void Rffi_CryptoAuth2_tryHandshake(RTypes_CryptoAuth2_t *ca,
                                   Message_t *c_msg,
                                   Allocator_t *alloc,
                                   bool requireAuth,
                                   RTypes_CryptoAuth2_TryHandshake_Ret_t *ret);

RTypes_CryptoAuth2_Session_t *Rffi_CryptoAuth2_newSession(RTypes_CryptoAuth2_t *ca,
                                                          Allocator_t *alloc,
                                                          const uint8_t *herPublicKey,
                                                          bool requireAuth,
                                                          const char *name,
                                                          bool useNoise);

Message_t *Rffi_CryptoAuth2_noiseTick(RTypes_CryptoAuth2_Session_t *sess, Allocator_t *alloc);

void Rffi_CryptoAuth2_setAuth(const String_t *password,
                              const String_t *login,
                              RTypes_CryptoAuth2_Session_t *caSession);

void Rffi_CryptoAuth2_resetIfTimeout(RTypes_CryptoAuth2_Session_t *session);

void Rffi_CryptoAuth2_reset(RTypes_CryptoAuth2_Session_t *caSession);

RTypes_CryptoAuth_State_t Rffi_CryptoAuth2_getState(RTypes_CryptoAuth2_Session_t *session);

void Rffi_CryptoAuth2_getHerPubKey(const RTypes_CryptoAuth2_Session_t *session, uint8_t *pkOut);

void Rffi_CryptoAuth2_getHerIp6(const RTypes_CryptoAuth2_Session_t *session, uint8_t *ipOut);

String_t *Rffi_CryptoAuth2_getName(const RTypes_CryptoAuth2_Session_t *session, Allocator_t *alloc);

void Rffi_CryptoAuth2_getPubKey(const RTypes_CryptoAuth2_t *ca, uint8_t *pkOut);

void Rffi_CryptoAuth2_stats(const RTypes_CryptoAuth2_Session_t *session,
                            RTypes_CryptoStats_t *statsOut);

uint32_t Rffi_CryptoAuth2_cjdnsVer(const RTypes_CryptoAuth2_Session_t *session);

/**
 * Helper function to lock the Global C Lock, used only within libuv's core runtime (unix and windows).
 */
Rffi_Glock_guard *Rffi_glock(void);

/**
 * Helper function to unlock the Global C Lock, as noted above.
 */
void Rffi_gunlock(Rffi_Glock_guard *guard);

/**
 * Create a new EventLoop data repository.
 */
Rffi_EventLoop *Rffi_mkEventLoop(Allocator_t *alloc);

/**
 * Return some EventLoop's ref counter to C.
 */
unsigned int Rffi_eventLoopRefCtr(Rffi_EventLoop *event_loop);

/**
 * Get the full filesystem path of the current running executable.
 */
int32_t Rffi_exepath(const char **out, Allocator_t *alloc);

/**
 * Spawn a new child process, and monitors its result.
 */
int32_t Rffi_spawn(const char *file,
                   const char *const *args,
                   int num_args,
                   Allocator_t *_alloc,
                   Rffi_EventLoop *event_loop,
                   void (*cb)(long, int));

/**
 * Spawn a timer task for a timeout or interval, that calls some callback whenever it triggers.
 */
void Rffi_setTimeout(const Rffi_TimerTx **out_timer_tx,
                     void (*cb)(void*),
                     void *cb_context,
                     unsigned long timeout_millis,
                     bool repeat,
                     Rffi_EventLoop *event_loop,
                     Allocator_t *alloc);

/**
 * Reset a timer task to change its timing.
 */
int Rffi_resetTimeout(const Rffi_TimerTx *timer_tx, unsigned long timeout_millis);

/**
 * Cancel a timer task.
 */
int Rffi_clearTimeout(const Rffi_TimerTx *timer_tx);

/**
 * Return 1 if a timer task is still running, 0 otherwise.
 */
int Rffi_isTimeoutActive(const Rffi_TimerTx *timer_tx);

/**
 * Cancel all timer tasks.
 */
void Rffi_clearAllTimeouts(Rffi_EventLoop *event_loop);

/**
 * Convert IPv4 and IPv6 addresses from binary to text form.
 */
int32_t Rffi_inet_ntop(bool is_ip6, const void *addr, uint8_t *dst, uint32_t dst_sz);

/**
 * Convert IPv4 and IPv6 addresses from text to binary form.
 */
int32_t Rffi_inet_pton(bool is_ip6, const char *src, uint8_t *addr);

/**
 * Get a list of available network interfaces for the current machine.
 */
int32_t Rffi_interface_addresses(const Rffi_NetworkInterface **out, Allocator_t *alloc);

/**
 * Non-monotonic nanosecond time, which has no relationship to any wall clock.
 */
uint64_t Rffi_hrtime(void);

/**
 * Monotonic millisecond time.
 */
uint64_t Rffi_now_ms(void);

RTypes_IfWrapper_t Rffi_testwrapper_create(Allocator_t *a);

RTypes_IfWrapper_t Rffi_android_create(Allocator_t *a);

void Rffi_panic(const char *msg);

void Rffi_setLogger(Log_t *l);

RTypes_Error_t *Rffi_error(const char *msg, Allocator_t *alloc);

RTypes_Error_t *Rffi_error_fl(const char *msg, const char *file, int line, Allocator_t *alloc);

const char *Rffi_printError(RTypes_Error_t *e, Allocator_t *alloc);

#endif /* rffi_H */
