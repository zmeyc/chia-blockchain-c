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

struct CalculateIPItersResult {
    enum {
        CIPI_SUCCESS,
        CIPI_SP_INDEX_TOO_HIGH,
        CIPI_INVALID_SP_ITERS_FOR_SSI, // Invalid sp_iters for this sub_slot_iters
        // Required_iters is not below the sp_interval_iters or not >0
        CIPI_INVALID_REQUIRED_ITERS,
    } code;
    union {
        uint64_t value;
        struct InvalidSPItersForSSI {
            uint64_t sp_iters;
        } invalid_sp_iters_for_ssi;
        struct InvalidRequiredIters {
            uint64_t sp_interval_iters;
        } invalid_required_iters;
    };
};

struct CalculateIPItersResult calculate_ip_iters(
    const struct ConsensusConstants *constants,
    uint64_t sub_slot_iters,
    uint8_t signage_point_index,
    uint64_t required_iters
);

uint64_t calculate_iterations_quality(
    uint128_t difficulty_constant_factor,
    const struct Bytes32 *quality_string,
    int size,
    uint64_t difficulty,
    const struct Bytes32 *cc_sp_output_hash
);

