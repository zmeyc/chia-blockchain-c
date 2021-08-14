#include "sized_bytes.h"

#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

#include "../../util/types.h"

static int bytes_from_hex(uint8_t *out, size_t size, const char *hex);

struct Bytes32 bytes32_from_hex(const char *hex, int *error_code)
{
    struct Bytes32 out;
    int result = bytes_from_hex(out.value, ARRAY_SIZE(out.value), hex);
    if (error_code) {
        *error_code = result;
    } else {
        if (0 != result)
            abort();
    }
    return out;
}

uint8_t *bytes_from_hex_alloc(const char *hex)
{
    assert(hex != NULL);

    size_t len = 0;
    char c;
    for (size_t at = 0; (c = hex[at]) != '\0'; ++at) {
        if (isspace(c))
            continue;
        ++len;
    }
    len /= 2;
    if (len == 0)
        return NULL;
    
    uint8_t *buf = malloc(len);
    if (!buf)
        return NULL;
    if (0 != bytes_from_hex(buf, len, hex)) {
        free(buf);
        return NULL;
    }

    return buf;
}

static inline uint8_t byte_from_char(const char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return (uint8_t) -1;
}

static int bytes_from_hex(uint8_t *out, size_t size, const char *hex)
{
    assert(hex != NULL);

    size_t hex_at = 0;
    size_t out_at = 0;
    char c;
    while ((c = hex[hex_at]) != '\0') {
        if (isspace(c)) {
            ++hex_at;
            continue;
        }
        uint8_t b1 = byte_from_char(c);
        if (b1 == (uint8_t) -1)
            return 1;
        ++hex_at;

        c = hex[hex_at];
        if ('\0' == c)
            return 1;
        uint8_t b2 = byte_from_char(c);
        if (b2 == (uint8_t) -1)
            return 1;

        uint8_t b = (b1 << 4) | b2;
        if (out_at >= size)
            return 1;
        out[out_at++] = b;

        ++hex_at;
    }
    return !(out_at == size && hex[hex_at] == '\0');
}

