#include <stdint.h>

// Given the plot size parameter k (which is between 32 and 59), computes the
// expected size of the plot in bytes (times a constant factor). This is based on efficient encoding
// of the plot, and aims to be scale agnostic, so larger plots don't
// necessarily get more rewards per byte. The +1 is added to give half a bit more space per entry, which
// is necessary to store the entries in the plot.
uint64_t _expected_plot_size(int k)
{
    return ((2ull * k) + 1) * (1ull << (k - 1));
}

