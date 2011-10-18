#ifndef AVERAGEROLLER_H
#define AVERAGEROLLER_H

#include <stdint.h>

#include "memory/MemAllocator.h"

struct AverageRoller;

/**
 * Create a new AverageRoller.
 *
 * @param windowSeconds the number of seconds the average should run over.
 * @return a new roller.
 */
struct AverageRoller* AverageRoller_new(const uint32_t windowSeconds, const struct MemAllocator* allocator);

/**
 * Get the average.
 *
 * @param roller the roller context created by AverageRoller_new()
 * @return the average over the last windowSeconds active seconds.
 */
uint32_t AverageRoller_getAverage(struct AverageRoller* roller);

/**
 * Update the roller with a new entry and get the average.
 * If there are no new updates then past seconds do not get trimmed off and
 * the average stays the same. This, though destroying it's mathmatical correctness,
 * provides some forgiveness to the average since it will not drop to 0 if windowSeconds
 * seconds elapse with no update.
 *
 * @param roller the roller context created by AverageRoller_new()
 * @param newEntry the a new number to be factored into the average.
 * @return the average over the last windowSeconds active seconds.
 */
uint32_t AverageRoller_update(struct AverageRoller* roller, const uint32_t newEntry);

#endif
