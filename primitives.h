#pragma once

// Aliases for primitive types.

#include <cstdint>
#include <limits>

namespace primitives {

using length_t = uint64_t; // as in Segment lengths.
using point_id_t = uint32_t;
using space_t = double; // as in x, y coordinates.

using depth_t = int; // as in maximum quadtree depth.
using quadrant_t = int; // as in quadtree quadrant index.
using morton_key_t = uint64_t;
using grid_t = int; // for indexing a grid produced by a quadtree at a certain depth.

} // namespace primitives

