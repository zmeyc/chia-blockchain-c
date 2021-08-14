#include <stdio.h>
#include <stdint.h>

int test_suites_run;
int tests_run;

void t_dump_bytes(uint8_t *bytes, size_t size)
{
    for (size_t i = 0; i < size; ++i) {
        fprintf(stderr, "%02x", bytes[i]);
    }
    fprintf(stderr, "\n");
}

