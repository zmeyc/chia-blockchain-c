#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "tests.h"
#include "consensus/default_constants.h"
#include "consensus/pot_iterations.h"
#include "consensus/pos_quality.h"
#include "util/endian.h"
#include "util/hash.h"

static struct ConsensusConstants test_constants;

static const char *test_is_overflow_block()
{
    const struct IsOverflowBlockResult r1 = is_overflow_block(&test_constants, 27);
    t_assert(r1.code == IOB_SUCCESS && !r1.value);

    const struct IsOverflowBlockResult r2 = is_overflow_block(&test_constants, 28);
    t_assert(r2.code == IOB_SUCCESS && !r2.value);

    const struct IsOverflowBlockResult r3 = is_overflow_block(&test_constants, 29);
    t_assert(r3.code == IOB_SUCCESS && r3.value);

    const struct IsOverflowBlockResult r4 = is_overflow_block(&test_constants, 30);
    t_assert(r4.code == IOB_SUCCESS && r4.value);

    const struct IsOverflowBlockResult r5 = is_overflow_block(&test_constants, 31);
    t_assert(r5.code == IOB_SUCCESS && r5.value);

    const struct IsOverflowBlockResult r6 = is_overflow_block(&test_constants, 32);
    t_assert(r6.code == IOB_SP_INDEX_TOO_HIGH);

    return NULL;
}

static const char *test_calculate_sp_iters()
{
    uint64_t ssi = 100001ull * 64 * 4;
    
    const struct CalculateSPItersResult r1 = calculate_sp_iters(&test_constants, ssi, 32);
    t_assert(r1.code == CSPI_SP_INDEX_TOO_HIGH);

    const struct CalculateSPItersResult r2 = calculate_sp_iters(&test_constants, ssi, 31);
    t_assert(r2.code == CSPI_SUCCESS);

    return NULL;
}

static const char *test_calculate_ip_iters()
{
    uint64_t ssi = 100001ull * 64 * 4;
    uint64_t sp_interval_iters = ssi / test_constants.num_sps_per_sub_slot;

    // Invalid signage point index
    struct CalculateIPItersResult r1 = calculate_ip_iters(&test_constants, ssi, 123, 100000);
    t_assert(r1.code == CIPI_SP_INDEX_TOO_HIGH);

    uint64_t sp_iters = sp_interval_iters * 13;
    // required_iters too high
    struct CalculateIPItersResult r2 = calculate_ip_iters(&test_constants, ssi, sp_interval_iters, sp_interval_iters);
    t_assert(r2.code == CIPI_INVALID_REQUIRED_ITERS);

    // required_iters too high
    struct CalculateIPItersResult r3 = calculate_ip_iters(&test_constants, ssi, sp_interval_iters, sp_interval_iters * 12);
    t_assert(r3.code == CIPI_INVALID_REQUIRED_ITERS);

    // required_iters too low (0)
    struct CalculateIPItersResult r4 = calculate_ip_iters(&test_constants, ssi, sp_interval_iters, 0);
    t_assert(r4.code == CIPI_INVALID_REQUIRED_ITERS);

    uint64_t required_iters = sp_interval_iters - 1;

    struct CalculateIPItersResult ip_iters_result = calculate_ip_iters(&test_constants, ssi, 13, required_iters);
    t_assert(ip_iters_result.code == CIPI_SUCCESS);
    uint64_t ip_iters = ip_iters_result.value;
    t_assert(ip_iters == sp_iters + test_constants.num_sp_intervals_extra * sp_interval_iters + required_iters);

    required_iters = 1;
    ip_iters_result = calculate_ip_iters(&test_constants, ssi, 13, required_iters);
    t_assert(ip_iters_result.code == CIPI_SUCCESS);
    ip_iters = ip_iters_result.value;
    t_assert(ip_iters == sp_iters + test_constants.num_sp_intervals_extra * sp_interval_iters + required_iters);

    required_iters = ssi * 4 / 300;
    ip_iters_result = calculate_ip_iters(&test_constants, ssi, 13, required_iters);
    t_assert(ip_iters_result.code == CIPI_SUCCESS);
    ip_iters = ip_iters_result.value;
    t_assert(ip_iters == sp_iters + test_constants.num_sp_intervals_extra * sp_interval_iters + required_iters);
    t_assert(sp_iters < ip_iters);

    // Overflow
    sp_iters = sp_interval_iters * (test_constants.num_sps_per_sub_slot - 1);
    ip_iters_result = calculate_ip_iters(
        &test_constants,
        ssi,
        test_constants.num_sps_per_sub_slot - 1,
        required_iters
    );
    t_assert(ip_iters_result.code == CIPI_SUCCESS);
    ip_iters = ip_iters_result.value;
    t_assert(ip_iters == (sp_iters + test_constants.num_sp_intervals_extra * sp_interval_iters + required_iters) % ssi);
    t_assert(sp_iters > ip_iters);

    return NULL;
}

// Tests that the percentage of blocks won is proportional to the space of each farmer,
// with the assumption that all farmers have access to the same VDF speed.
static const char *test_win_percentage()
{
    enum { NUM_KS = 5, PLOT_COUNT = 100 };
    // Starting from k32 at index 0
    const uint32_t farmer_ks[NUM_KS] = { PLOT_COUNT, PLOT_COUNT, PLOT_COUNT, PLOT_COUNT, PLOT_COUNT };
    uint64_t farmer_space[NUM_KS];
    uint64_t total_space = 0;
    for (size_t i = 0; i < NUM_KS; ++i) {
        const uint64_t space = _expected_plot_size(32 + i) * farmer_ks[i];
        farmer_space[i] = space;
        total_space += space;
    }
    double percentage_space[NUM_KS];
    for (size_t i = 0; i < NUM_KS; ++i) {
        percentage_space[i] = (double)farmer_space[i] / total_space;
    }
    uint64_t wins[NUM_KS] = { 0 };
    uint64_t total_wins = 0;

    const uint32_t total_slots = 50;
    const uint32_t num_sps = 16;
    const uint64_t sp_interval_iters = 100000000ull / 32;
    const uint64_t difficulty = 500000000000ull;
    for (uint32_t slot_index = 0; slot_index < total_slots; ++slot_index) {
        for (uint32_t sp_index = 0; sp_index < num_sps; ++sp_index) {
            const uint32_t buf[] = {
              host_to_big_endian_u32(slot_index),
              host_to_big_endian_u32(sp_index)
            };
            struct Bytes32 sp_hash = std_hash((const uint8_t *)buf, sizeof buf);
            for (size_t i = 0; i < NUM_KS; ++i) {
                const uint8_t k = 32 + i;
                const uint8_t count = farmer_ks[i];
                for (uint8_t farmer_index = 0; farmer_index < count; ++farmer_index) {
                    #pragma pack(push, 1)
                    struct {
                        uint32_t slot_index_be;
                        uint8_t k;
                        uint32_t farmer_index; // uint8_t zeros[PLOT_COUNT];
                    } buf2;
                    #pragma pack(pop)
                    buf2.slot_index_be = host_to_big_endian_u32(slot_index);
                    buf2.k = k;
                    buf2.farmer_index = farmer_index; // memset(buf2.zeros, 0, farmer_index);
                    const struct Bytes32 quality = std_hash(
                        (const uint8_t *)&buf2,
                        sizeof buf2 // sizeof(buf2.slot_index_be) + sizeof(buf2.k) + farmer_index
                    );
                    const uint64_t required_iters = calculate_iterations_quality(
                        (uint128_t)1 << 25,
                        &quality,
                        k,
                        difficulty,
                        &sp_hash);
                    if (required_iters < sp_interval_iters) {
                        ++wins[i];
                        ++total_wins;
                    }

                }
            }
        }
    }

    for (size_t i = 0; i < NUM_KS; ++i) {
        const double win_percentage = (double)wins[i] / total_wins;
        // Win rate is proportional to percentage of space
        const double win_rate_space_diff = fabs(win_percentage - percentage_space[i]);
        t_assert(win_rate_space_diff < 0.01);
    }

    return NULL;
}

const char *test_pot_iterations_init()
{
    test_constants = get_default_constants();
    test_constants.num_sps_per_sub_slot = 32;
    test_constants.sub_slot_time_target = 300;
    return NULL;
}

const char *test_pot_iterations_cleanup()
{
    return NULL;
}

const char *test_pot_iterations()
{
    t_run("is_overflow_block", test_is_overflow_block);
    t_run("calculate_sp_iters", test_calculate_sp_iters);
    t_run("calculate_ip_iters", test_calculate_ip_iters);
    t_run("win_percentage", test_win_percentage);

    return NULL;
}

