#ifndef RTypes_H
#define RTypes_H

// This file is generated from src/rtypes.rs using cbindgen

#include "RTypesPrefix.h"

typedef enum {
  /**
   * New CryptoAuth session, has not sent or received anything
   */
  RTypes_CryptoAuth_State_t_Init = 0,
  /**
   * Sent a hello message, waiting for reply
   */
  RTypes_CryptoAuth_State_t_SentHello = 1,
  /**
   * Received a hello message, have not yet sent a reply
   */
  RTypes_CryptoAuth_State_t_ReceivedHello = 2,
  /**
   * Received a hello message, sent a key message, waiting for the session to complete
   */
  RTypes_CryptoAuth_State_t_SentKey = 3,
  /**
   * Sent a hello message, received a key message, may or may not have sent some data traffic
   * but no data traffic has yet been received
   */
  RTypes_CryptoAuth_State_t_ReceivedKey = 4,
  /**
   * Received data traffic, session is in run state
   */
  RTypes_CryptoAuth_State_t_Established = 100,
} RTypes_CryptoAuth_State_t;

typedef struct {
  Iface_t *internal;
  Iface_t *external;
} RTypes_IfWrapper_t;

typedef struct {
  uintptr_t len;
  String_t **items;
} RTypes_StrList_t;

typedef struct {
  /**
   * Number of packets which were lost
   */
  uint64_t lost_packets;
  /**
   * Number of packets which were received but could not be validated
   */
  uint64_t received_unexpected;
  /**
   * Number of packets which were received (since last session setup)
   */
  uint64_t received_packets;
  /**
   * Number of packets which were received that were duplicates
   */
  uint64_t duplicate_packets;
  /**
   * True if the session is using the Noise protocol
   */
  bool noise_proto;
} RTypes_CryptoStats_t;

typedef struct {
  Iface_t *plaintext;
  Iface_t *ciphertext;
} RTypes_CryptoAuth2_Session_t;

typedef struct {
  RTypes_IfWrapper_t a;
  RTypes_StrList_t b;
  RTypes_CryptoAuth_State_t c;
  RTypes_CryptoStats_t d;
  RTypes_CryptoAuth2_Session_t e;
} RTypes_ExportMe;

#endif /* RTypes_H */
