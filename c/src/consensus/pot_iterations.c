#include <assert.h>

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

