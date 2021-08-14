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

