#include <stdlib.h>

#include "tests.h"
#include "consensus/default_constants.h"
#include "consensus/pot_iterations.h"

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

    return NULL;
}

