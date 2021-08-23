#pragma once

#include <stdint.h>

#include "sized_bytes.h"

struct SubEpochSummary {
    struct Bytes32 prev_subepoch_summary_hash;
    struct Bytes32 reward_chain_hash; // hash of reward chain at end of last segment
    uint8_t num_blocks_overflow; // How many more blocks than 384*(N-1)
    struct {
        uint64_t value;
        bool has_value;
    } new_difficulty; // Only once per epoch (diff adjustment)
    struct {
        uint64_t value;
        bool has_value;
    } new_sub_slot_iters; // Only once per epoch (diff adjustment)
};

