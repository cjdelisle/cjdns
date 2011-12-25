#ifndef ERROR_H
#define ERROR_H

/** No error, everything is ok. */
#define Error_NONE                 0

/** The switch label was malformed. */
#define Error_MALFORMED_ADDRESS    1

/** Packet dropped because link is congested. */
#define Error_FLOOD                2

/** Packet dropped because node has oversent it's limit. */
#define Error_LINK_LIMIT_EXCEEDED  3

/** Message too big to send. */
#define Error_OVERSIZE_MESSAGE     4

/** Message smaller than expected headers. */
#define Error_UNDERSIZE_MESSAGE    5

/** Authentication failed. */
#define Error_AUTHENTICATION       6

/** Header is invalid or checksum failed. */
#define Error_INVALID              7

/** Message could not be sent to it's destination through no fault of the sender. */
#define Error_UNDELIVERABLE        8


#endif
