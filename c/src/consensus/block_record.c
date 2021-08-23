#include "block_record.h"

#include "constants.h"

bool block_record_is_transaction_block(const struct BlockRecord *block_record)
{
    return block_record->timestamp.has_value;
}

bool block_record_first_in_sub_slot(const struct BlockRecord *block_record)
{
    return block_record->finished_challenge_slot_hashes != NULL;
}

bool block_record_is_challenge_block(const struct BlockRecord *block_record, const struct ConsensusConstants *constants)
{
    return block_record->deficit == constants->min_blocks_per_challenge_block - 1;
}

struct SPSubSlotTotalItersResult block_record_sp_sub_slot_total_iters(const struct BlockRecord *block_record, const struct ConsensusConstants *constants)
{
    const struct CalculateIPItersResult ip_iters_result = block_record_ip_iters(block_record, constants);
    if (ip_iters_result.code != CIPI_SUCCESS) {
        return (struct SPSubSlotTotalItersResult) {
            .code = SPSSTI_CIPI_ERROR,
            .cipi_error = ip_iters_result
        };
    }
    uint128_t ip_iters = ip_iters_result.value;

    uint128_t total_iters;
    if (block_record->overflow)
        total_iters = block_record->total_iters - ip_iters - block_record->sub_slot_iters;
    else
        total_iters = block_record->total_iters - ip_iters;
    return (struct SPSubSlotTotalItersResult) {
        .value = total_iters
    };
}

struct IPSubSlotTotalItersResult block_record_ip_sub_slot_total_iters(const struct BlockRecord *block_record, const struct ConsensusConstants *constants)
{
    const struct CalculateIPItersResult ip_iters_result = block_record_ip_iters(block_record, constants);
    if (ip_iters_result.code != CIPI_SUCCESS) {
        return (struct IPSubSlotTotalItersResult) {
            .code = IPSSTI_CIPI_ERROR,
            .cipi_error = ip_iters_result
        };
    }
    uint128_t ip_iters = ip_iters_result.value;

    return (struct IPSubSlotTotalItersResult) {
        .value = block_record->total_iters - ip_iters
    };
}

struct CalculateSPItersResult block_record_sp_iters(const struct BlockRecord *block_record, const struct ConsensusConstants *constants)
{
    return calculate_sp_iters(constants, block_record->sub_slot_iters, block_record->signage_point_index);
}

struct CalculateIPItersResult block_record_ip_iters(const struct BlockRecord *block_record, const struct ConsensusConstants *constants)
{
    return calculate_ip_iters(
        constants,
        block_record->sub_slot_iters,
        block_record->signage_point_index,
        block_record->required_iters
    );
}

struct SPTotalItersResult block_record_sp_total_iters(const struct BlockRecord *block_record, const struct ConsensusConstants *constants)
{
    const struct SPSubSlotTotalItersResult spssti_result = block_record_sp_sub_slot_total_iters(block_record, constants);
    if (spssti_result.code != SPSSTI_SUCCESS) {
        return (struct SPTotalItersResult) {
            .code = SPTI_SPSSTI_ERROR,
            .spssti_error = spssti_result
        };
    }
    uint128_t sp_sub_slot_total_iters = spssti_result.value;

    const struct CalculateSPItersResult cspi_result = block_record_sp_iters(block_record, constants);
    if (cspi_result.code != CSPI_SUCCESS) {
        return (struct SPTotalItersResult) {
            .code = SPTI_CSPI_ERROR,
            .cspi_error = cspi_result
        };
    }
    uint64_t sp_iters = cspi_result.value;

    return (struct SPTotalItersResult) {
        .value = sp_sub_slot_total_iters + sp_iters
    };
}

