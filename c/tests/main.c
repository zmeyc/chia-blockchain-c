#include <stdio.h>

#include "tests.h"

#include "consensus/test_pot_iterations.h"
#include "types/blockchain_format/test_sized_bytes.h"

static const char *all_tests()
{
    t_suite_run("pot_iterations", test_pot_iterations);
    t_suite_run("sized_bytes", test_sized_bytes);

    return 0;
}

int main(void)
{
    const char *result = all_tests();

    if (result == NULL) {
        printf("ALL TESTS PASSED\n");
    } else {
        fprintf(stderr, "%s\n", result);
        return 1;
    }
    printf("Test suites run: %d (%d tests)\n", test_suites_run, tests_run);

    return result != 0;
}

