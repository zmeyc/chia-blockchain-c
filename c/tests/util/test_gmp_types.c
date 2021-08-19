#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#include <gmp.h>

#include "tests.h"
#include "util/types.h"
#include "util/gmp_types.h"

static const char *test_ui64_conversion()
{
    uint64_t val1 = UINT64_MAX;
    char buf1[256];
    snprintf(buf1, sizeof buf1, "%" PRIu64, val1);

    uint64_t val2 = UINT64_MAX - 1;
    char buf2[256];
    snprintf(buf2, sizeof buf2, "%" PRIu64, val2);

    uint64_t val3 = 1;
    char buf3[256];
    snprintf(buf3, sizeof buf3, "%" PRIu64, val3);

    mpz_t v;
    mpz_init(v);

    mpz_set_ui64(v, val1);
    char buf1_gmp[256];
    gmp_snprintf(buf1_gmp, sizeof buf1_gmp, "%Zu", v);
    t_assert(0 == strcmp(buf1, buf1_gmp));
    t_assert(UINT64_MAX == mpz_get_ui64(v));

    mpz_set_ui64(v, val2);
    char buf2_gmp[256];
    gmp_snprintf(buf2_gmp, sizeof buf2_gmp, "%Zu", v);
    t_assert(0 == strcmp(buf2, buf2_gmp));
    t_assert(UINT64_MAX - 1 == mpz_get_ui64(v));

    mpz_set_ui64(v, val3);
    char buf3_gmp[256];
    gmp_snprintf(buf3_gmp, sizeof buf3_gmp, "%Zu", v);
    t_assert(0 == strcmp(buf3, buf3_gmp));
    t_assert(1 == mpz_get_ui64(v));

    mpz_clear(v);

    return 0;
}

static const char *test_ui128_conversion()
{
    uint128_t uint128_max;
    memset(&uint128_max, 0xff, sizeof(uint128_max));

    uint128_t val1 = uint128_max;
    char buf1[256];
    snprintf(buf1, sizeof buf1, "%016" PRIx64 "%016" PRIx64, (uint64_t) (val1 >> 64), (uint64_t) val1 & 0xFFFFFFFFFFFFFFFF);

    uint128_t val2 = uint128_max - 1;
    char buf2[256];
    snprintf(buf2, sizeof buf2, "%016" PRIx64 "%016" PRIx64, (uint64_t) (val2 >> 64), (uint64_t) val2 & 0xFFFFFFFFFFFFFFFF);

    uint128_t val3 = 1;
    char buf3[256];
    snprintf(buf3, sizeof buf3, "%016" PRIx64 "%016" PRIx64, (uint64_t) (val3 >> 64), (uint64_t) val3 & 0xFFFFFFFFFFFFFFFF);

    mpz_t v;
    mpz_init(v);

    mpz_set_ui128(v, val1);
    char buf1_gmp[256];
    gmp_snprintf(buf1_gmp, sizeof buf1_gmp, "%032Zx", v);
    t_assert(0 == strcmp(buf1, buf1_gmp));

    mpz_set_ui128(v, val2);
    char buf2_gmp[256];
    gmp_snprintf(buf2_gmp, sizeof buf2_gmp, "%032Zx", v);
    t_assert(0 == strcmp(buf2, buf2_gmp));

    mpz_set_ui128(v, val3);
    char buf3_gmp[256];
    gmp_snprintf(buf3_gmp, sizeof buf3_gmp, "%032Zx", v);
    t_assert(0 == strcmp(buf3, buf3_gmp));

    mpz_clear(v);

    return 0;
}

const char *test_gmp_types_init()
{
    return NULL;
}

const char *test_gmp_types_cleanup()
{
    return NULL;
}

const char *test_gmp_types()
{
    t_run("ui64_conversion", test_ui64_conversion);
    t_run("ui128_conversion", test_ui128_conversion);

    return NULL;
}

