#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <gmp.h>

#include "../util/gmp_types.h"
#include "../util/hash.h"
#include "../util/math_util.h"

#include "constants.h"
#include "pos_quality.h"
#include "pot_iterations.h"

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

// Calculates the number of iterations from the quality. This is derives as the difficulty times the constant factor
// times a random number between 0 and 1 (based on quality string), divided by plot size.
uint64_t calculate_iterations_quality(
    uint128_t difficulty_constant_factor,
    const struct Bytes32 *quality_string,
    int size,
    uint64_t difficulty,
    const struct Bytes32 *cc_sp_output_hash
) {
    uint8_t buf[sizeof quality_string->value + sizeof cc_sp_output_hash->value];
    memcpy(buf,
        quality_string->value, sizeof quality_string->value);
    memcpy(buf + sizeof quality_string->value,
        cc_sp_output_hash->value, sizeof cc_sp_output_hash->value);
    struct Bytes32 sp_quality_string = std_hash(buf, sizeof buf);

    mpz_t difficulty_constant_factor_mpz;
    mpz_init(difficulty_constant_factor_mpz);
    mpz_set_ui128(difficulty_constant_factor_mpz, difficulty_constant_factor);

    mpz_t sp_quality;
    mpz_init(sp_quality);
    mpz_set_ubytes(
        sp_quality,
        sp_quality_string.value, sizeof sp_quality_string.value,
        MPZ_ENDIAN_MOST_SIGNIFICANT_FIRST
    );

    mpz_t expected_plot_size;
    mpz_init(expected_plot_size);
    mpz_set_ui64(expected_plot_size, _expected_plot_size(size));

    mpz_t res;
    mpz_init(res);
    mpz_set_ui64(res, difficulty);
    mpz_mul(res, res, difficulty_constant_factor_mpz);
    mpz_mul(res, res, sp_quality);

    mpz_t denom;
    mpz_init(denom);
    mpz_ui_pow_ui(denom, 2, 256);
    mpz_mul(denom, denom, expected_plot_size);

    mpz_tdiv_q(res, res, denom);

    uint64_t iters = mpz_get_ui64(res);

    mpz_clear(denom);
    mpz_clear(res);
    mpz_clear(expected_plot_size);
    mpz_clear(sp_quality);
    mpz_clear(difficulty_constant_factor_mpz);

    return MAX(iters, 1);
}

