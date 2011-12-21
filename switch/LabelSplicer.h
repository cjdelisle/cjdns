#ifndef LABEL_SPLICER_H
#define LABEL_SPLICER_H

#include <stdint.h>

/**
 * Splice a label and a label fragment together.
 *
 */
uint64_t LabelSplicer_splice(uint64_t goHere_be, uint64_t viaHere_be);

/**
 * Get the label for a particular destination from a given source.
 * This needs to be called before handing out a label because if a source interface is
 * represented using more bits than the destination interface, the destination interface
 * must be padded out so that the switch will find the source and destination labels compatable.
 *
 * @param target_be the label for the location to send to in big endian.
 * @param whoIsAsking_be the label for the node which we are sending the target to in big endian.
 * @return the modified target for that node in big endian.
 */
uint64_t LabelSplicer_getLabelFor(uint64_t target_be, uint64_t whoIsAsking_be);

#endif
