#pragma once

#include <stdint.h>
#include <stdbool.h>

struct ConsensusConstants;

struct IsOverflowBlockResult {
    enum {
        IOB_SUCCESS,
        IOB_SP_INDEX_TOO_HIGH
    } code;
    bool value;
};

struct IsOverflowBlockResult is_overflow_block(const struct ConsensusConstants *constants, uint8_t signage_point_index);
uint64_t calculate_sp_interval_iters(const struct ConsensusConstants *constants, uint64_t sub_slot_iters);

