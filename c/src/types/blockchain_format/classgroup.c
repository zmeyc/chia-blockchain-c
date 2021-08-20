#include <assert.h>
#include <string.h>

#include "classgroup.h"

#include "../../consensus/constants.h"
#include "../../util/math_util.h"

static const size_t classgroup_element_size = 100;

struct ClassgroupElement classgroup_element_from_bytes(const uint8_t *data, size_t size)
{
    struct ClassgroupElement element;
    assert(sizeof element.data.value == classgroup_element_size);

    if (size < classgroup_element_size) {
        memcpy(element.data.value, data, size);
        memset(element.data.value + size, 0, classgroup_element_size - size);
    } else {
        memcpy(element.data.value, data, MIN(sizeof element.data.value, size));
    }
    return element;
}

struct ClassgroupElement classgroup_element_get_default_element()
{
    // Bit 3 in the first byte of serialized compressed form indicates if
    // it's the default generator element.
    uint8_t buf[] = { 0x08 };
    return classgroup_element_from_bytes(buf, sizeof buf);
}

size_t classgroup_element_get_size(const struct ConsensusConstants *constants)
{
    return 100;
}

