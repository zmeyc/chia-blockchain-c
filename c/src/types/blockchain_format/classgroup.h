#pragma once

#include "sized_bytes.h"

struct ConsensusConstants;

// Represents a classgroup element (a,b,c) where a, b, and c are 512 bit signed integers. However this is using
// a compressed representation. VDF outputs are a single classgroup element. VDF proofs can also be one classgroup
// element (or multiple).
struct ClassgroupElement {
    struct Bytes100 data;
};

struct ClassgroupElement classgroup_element_from_bytes(const uint8_t *data, size_t size);
struct ClassgroupElement classgroup_element_get_default_element();
size_t classgroup_element_get_size(const struct ConsensusConstants *constants);

