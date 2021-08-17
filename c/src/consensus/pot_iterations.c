#include <assert.h>
#include <stdlib.h>

#include "pot_iterations.h"

#include "constants.h"

struct IsOverflowBlockResult is_overflow_block(const struct ConsensusConstants *constants, uint8_t signage_point_index)
{
    if (signage_point_index >= constants->num_sps_per_sub_slot)
        return (struct IsOverflowBlockResult) { .code = IOB_SP_INDEX_TOO_HIGH };
    return (struct IsOverflowBlockResult) {
        .value = signage_point_index >= constants->num_sps_per_sub_slot - constants->num_sp_intervals_extra
    };
}


uint64_t calculate_sp_interval_iters(const struct ConsensusConstants *constants, uint64_t sub_slot_iters)
{
    assert(sub_slot_iters % constants->num_sps_per_sub_slot == 0);
    return sub_slot_iters / constants->num_sps_per_sub_slot;
}

struct CalculateSPItersResult calculate_sp_iters(const struct ConsensusConstants *constants, uint64_t sub_slot_iters, uint8_t signage_point_index)
{
    if (signage_point_index >= constants->num_sps_per_sub_slot)
        return (struct CalculateSPItersResult) { .code = CSPI_SP_INDEX_TOO_HIGH };
    return (struct CalculateSPItersResult) {
        .value = calculate_sp_interval_iters(constants, sub_slot_iters) * signage_point_index
    };
}

struct CalculateIPItersResult calculate_ip_iters(
    const struct ConsensusConstants *constants,
    uint64_t sub_slot_iters,
    uint8_t signage_point_index,
    uint64_t required_iters
) {
    // Note that the SSI is for the block passed in, which might be in the previous epoch
    const struct CalculateSPItersResult cspi_res = calculate_sp_iters(constants, sub_slot_iters, signage_point_index);
    switch (cspi_res.code) {
        case CSPI_SUCCESS: break;
        case CSPI_SP_INDEX_TOO_HIGH: return (struct CalculateIPItersResult) { .code = CIPI_SP_INDEX_TOO_HIGH };
        default: abort();
    }
    uint64_t sp_iters = cspi_res.value;

    uint64_t sp_interval_iters = calculate_sp_interval_iters(constants, sub_slot_iters);
    if (sp_iters % sp_interval_iters != 0 || sp_iters >= sub_slot_iters) {
        return (struct CalculateIPItersResult) {
            .code = CIPI_INVALID_SP_ITERS_FOR_SSI,
            .invalid_sp_iters_for_ssi = (struct InvalidSPItersForSSI) {
                .sp_iters = sp_iters
            }
        };
    }

    if (required_iters >= sp_interval_iters || required_iters == 0) {
        return (struct CalculateIPItersResult) {
            .code = CIPI_INVALID_REQUIRED_ITERS,
            .invalid_required_iters = (struct InvalidRequiredIters) {
                .sp_interval_iters = sp_interval_iters
            }
        };
    }

    return (struct CalculateIPItersResult) {
        .value = (sp_iters + constants->num_sp_intervals_extra * sp_interval_iters + required_iters) % sub_slot_iters
    };
}
