#include <string.h>
#include <stdlib.h>

#include "tests.h"
#include "util/types.h"
#include "types/blockchain_format/sized_bytes.h"

static struct Bytes32 get_00_to_1f()
{
    struct Bytes32 ref;
    memset(ref.value, 0, sizeof(ref.value));
    for (size_t i = 0; i < ARRAY_SIZE(ref.value); ++i) {
        ref.value[i] = i;
    }
    return ref;
}

static const char *test_bytes32_zeros()
{
    int error_code;
    struct Bytes32 zeros = bytes32_from_hex("0000000000000000000000000000000000000000000000000000000000000000", &error_code);
    t_assert_msg("Byte32 has correct size", sizeof(zeros.value) == 32);
    t_assert_msg("Convert string of zeros to bytes32", error_code == 0);

    struct Bytes32 one_l = bytes32_from_hex("0100000000000000000000000000000000000000000000000000000000000000", &error_code);
    t_assert_msg("Convert 1 on the left to bytes32", error_code == 0);

    struct Bytes32 one_r = bytes32_from_hex("0000000000000000000000000000000000000000000000000000000000000001", &error_code);
    t_assert_msg("Convert 1 on the right to bytes32", error_code == 0);

    struct Bytes32 zeros_ref;
    memset(zeros_ref.value, 0, sizeof(zeros_ref.value));

    struct Bytes32 one_l_ref;
    memset(one_l_ref.value, 0, sizeof(one_l_ref.value));
    one_l_ref.value[0] = 1;

    struct Bytes32 one_r_ref;
    memset(one_r_ref.value, 0, sizeof(one_r_ref.value));
    one_r_ref.value[31] = 1;

    t_assert_msg("zeros == zeros_ref", 0 == memcmp(zeros.value, zeros_ref.value, sizeof(zeros.value)));
    t_assert_msg("one_l == one_l_ref", 0 == memcmp(one_l.value, one_l_ref.value, sizeof(one_l.value)));
    t_assert_msg("one_r == one_r_ref", 0 == memcmp(one_r.value, one_r_ref.value, sizeof(one_r.value)));

    t_assert_msg("one_l != zeros_ref", 0 != memcmp(one_l.value, zeros_ref.value, sizeof(one_l.value)));
    t_assert_msg("one_r != zeros_ref", 0 != memcmp(one_r.value, zeros_ref.value, sizeof(one_r.value)));

    return 0;
}

static const char *test_bytes32_parsing()
{
    struct Bytes32 ref = get_00_to_1f();

    int error_code;

    struct Bytes32 val1 = bytes32_from_hex("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f", &error_code);
    t_assert(error_code == 0);
    t_assert(0 == memcmp(val1.value, ref.value, sizeof(ref.value)));

    struct Bytes32 val2 = bytes32_from_hex(" 00 010203   04050607 \t\r\n 08090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f  ", &error_code);
    t_assert(error_code == 0);
    t_assert(0 == memcmp(val2.value, ref.value, sizeof(ref.value)));

    bytes32_from_hex("00010203040 5060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f", &error_code);
    t_assert(error_code == 1);

    bytes32_from_hex("00-0102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f", &error_code);
    t_assert(error_code == 1);

    return NULL;
}

static const char *test_bytes_alloc()
{
    struct Bytes32 ref = get_00_to_1f();

    uint8_t *val1 = bytes_from_hex_alloc("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f");
    t_assert(val1 != NULL);
    t_assert(val1 && 0 == memcmp(val1, ref.value, sizeof(ref.value)));
    free(val1);

    uint8_t *val2 = bytes_from_hex_alloc(" 00 010203   04050607 \t\r\n 08090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f  ");
    t_assert(val2 != NULL);
    t_assert(val2 && 0 == memcmp(val2, ref.value, sizeof(ref.value)));
    free(val2);

    uint8_t *val3 = bytes_from_hex_alloc("00010203040 5060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f");
    t_assert(val3 == NULL);

    uint8_t *val4 = bytes_from_hex_alloc("00-0102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f");
    t_assert(val4 == NULL);

    return NULL;
}

const char *test_sized_bytes_init()
{
    return NULL;
}

const char *test_sized_bytes_cleanup()
{
    return NULL;
}

const char *test_sized_bytes()
{
    t_run("bytes32_zeros", test_bytes32_zeros);
    t_run("bytes32_parsing", test_bytes32_parsing);
    t_run("bytes_alloc", test_bytes_alloc);

    return NULL;
}

