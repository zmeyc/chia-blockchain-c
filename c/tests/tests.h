#pragma once

#include <stdint.h>

extern int test_suites_run;
extern int tests_run;

#define S1(x) #x
#define S2(x) S1(x)

#define t_assert(test) \
    do {  \
        printf("Assert: %s\n", #test); \
        if (!(test)) \
            return __FILE__ ":" S2(__LINE__) ": assertion failed: " #test; \
    } while (0)
#define t_assert_msg(message, test) \
    do { \
        printf("Assert: %s\n", message); \
        if (!(test)) \
            return __FILE__ ":" S2(__LINE__) ": assertion failed: " message; \
    } while (0)

#define t_suite_run(name, test) \
    do { \
        printf("=== TEST SUITE: %s\n", (name)); \
        fflush(stdout); \
        const char *message = test(); \
        test_suites_run++; \
        if (message) \
            return (message); \
    } while (0)

#define t_run(name, test) \
    do { \
        printf("- Test: %s\n", (name)); \
        fflush(stdout); \
        const char *message = test(); \
        tests_run++; \
        if (message) \
            return (message); \
    } while (0)

void t_dump_bytes(uint8_t *bytes, size_t size);

