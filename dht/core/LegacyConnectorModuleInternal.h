#ifndef LEGACYCONNECTORMODULEINTERNAL_H
#define LEGACYCONNECTORMODULEINTERNAL_H

#include "dht/core/LegacyConnectorModule.h"

#define WANT4 1
#define WANT6 2

struct LegacyConnectorModule_context {

    /**
     * An absolute time at which dht_periodic() should be called next.
     * Number of seconds since epoch.
     */
    time_t whenToCallDHTPeriodic;

    benc_bstr_t myId;

    struct DHTModuleRegistry* registry;

    DHTHashFunction dhtHashFunction;

    DHTRandomBytes dhtRandomBytes;
};

/**
 * Need a global copy of context because dht.c is global.
 */
struct LegacyConnectorModule_context* context;

/*--------------------Structures needed for interface.--------------------*/

struct storage {
    unsigned char id[20];
    int numpeers, maxpeers;
    struct peer *peers;
    struct storage *next;
};

struct peer {
    time_t time;
    unsigned char ip[16];
    unsigned short len;
    unsigned short port;
};

/*--------------------For testing only.--------------------*/
void LegacyConnectorModuleInternal_setContext(struct LegacyConnectorModule_context* ctx);

#endif
