#pragma once

#include <stdint.h>

#include <gmp.h>

#include "types.h"

enum MPZEndian {
  MPZ_ENDIAN_NATIVE = 0,
  MPZ_ENDIAN_MOST_SIGNIFICANT_FIRST = 1,
  MPZ_ENDIAN_LEAST_SIGNIFICANT_FIRST = -1
};

uint64_t mpz_get_ui64(const mpz_t op);
void mpz_set_ui64(mpz_t rop, const uint64_t op);
void mpz_set_ui128(mpz_t rop, const uint128_t op);
void mpz_set_ubytes(mpz_t rop, const uint8_t *bytes, const size_t size, const enum MPZEndian endian);

