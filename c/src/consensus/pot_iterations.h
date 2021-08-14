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

struct CalculateSPItersResult {
    enum {
        CSPI_SUCCESS,
        CSPI_SP_INDEX_TOO_HIGH
    } code;
    uint64_t value;
};
struct CalculateSPItersResult calculate_sp_iters(const struct ConsensusConstants *constants, uint64_t sub_slot_iters, uint8_t signage_point_index);

