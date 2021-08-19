#include "relic/relic_md.h"

#include "../types/blockchain_format/sized_bytes.h"

// The standard hash used in many places.
struct Bytes32 std_hash(const uint8_t *bytes, const size_t size)
{
    struct Bytes32 result;
    md_map_sh256(result.value, bytes, size);
    return result;
}

