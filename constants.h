#pragma once

#include "primitives.h"

namespace constants {

constexpr auto invalid_point {std::numeric_limits<primitives::point_id_t>::max()};

constexpr int save_period {1};

constexpr primitives::depth_t max_tree_depth{21}; // maximum quadtree depth / level.

constexpr bool verbose {false};
constexpr bool write_best {true};
constexpr bool print_local_optima {true};
constexpr bool print_iterations {true};
constexpr bool verify {true};

} // namespace constants
