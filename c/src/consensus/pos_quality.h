#pragma once

#include <stdint.h>

// The actual space in bytes of a plot, is _expected_plot_size(k) * UI_ACTUAL_SPACE_CONSTANT_FACTO
// This is not used in consensus, only for display purposes
static const double UI_ACTUAL_SPACE_CONSTANT_FACTOR = 0.762;

uint64_t _expected_plot_size(int k);

