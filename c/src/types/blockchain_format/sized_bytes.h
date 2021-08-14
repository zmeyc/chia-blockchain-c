#pragma once

#include <stdio.h>
#include <stdint.h>

#pragma pack(push, 1)
struct Bytes32 {
    uint8_t value[32];
};
#pragma pack(pop)

uint8_t *bytes_from_hex_alloc(const char *hex);

struct Bytes32 bytes32_from_hex(const char *hex, int *error_code);

