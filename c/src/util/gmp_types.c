#include <stdint.h>
#include <stdlib.h>

#include <gmp.h>

#include "gmp_types.h"

#include "types.h"

uint64_t mpz_get_ui64(const mpz_t op)
{
    mpz_t uint64_max;
    mpz_init(uint64_max);
    mpz_set_ui64(uint64_max, UINT64_MAX);
    if (mpz_cmp(op, uint64_max) > 0)
        abort();
    mpz_clear(uint64_max);

    uint64_t result;
    mpz_export(&result, /* *countp */ NULL, /* order */ 1, /* size */ sizeof result, /* endian */ 0, /* nails */0, op);
    return result;
}

void mpz_set_ui64(mpz_t rop, const uint64_t op)
{
    mpz_import(rop, /* count */ 1, /* order */ 1, sizeof op, /* endian */ 0, /* nails */ 0, &op);
}

void mpz_set_ui128(mpz_t rop, const uint128_t op)
{
    mpz_import(rop, /* count */ 1, /* order */ 1, sizeof op, /* endian */ 0, /* nails */ 0, &op);
}

void mpz_set_ubytes(mpz_t rop, const uint8_t *bytes, const size_t size, const enum MPZEndian endian)
{
    mpz_import(rop, /* count */ 1, /* order */ 1, size, /* endian */ endian, /* nails */ 0, bytes);
}

