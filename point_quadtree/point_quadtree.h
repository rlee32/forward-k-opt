#pragma once

#include "Node.h"
#include "morton_keys.h"
#include <Tour.h>
#include <primitives.h>

namespace point_quadtree {

inline primitives::grid_t quadrant_x(primitives::quadrant_t q)
{
    // assuming "N" curve; see morton_keys::interleave_coordinates for an explanation.
    switch(q)
    {
        case 0:
        case 1:
        {
            return 0;
        }
        case 2:
        case 3:
        {
            return 1;
        }
        default: return 0;
    }
}

inline primitives::grid_t quadrant_y(primitives::quadrant_t q)
{
    // assuming "N" curve; see morton_keys::interleave_coordinates for an explanation.
    switch(q)
    {
        case 0:
        case 2:
        {
            return 0;
        }
        case 1:
        case 3:
        {
            return 1;
        }
        default: return 0;
    }
}

inline void insert_point(
    const std::vector<primitives::morton_key_t>& morton_keys
    , primitives::point_id_t point_id
    , Node& root
    , const Domain& domain)
{
    auto point_destination {&root};
    primitives::depth_t depth {0};
    primitives::grid_t x {0};
    primitives::grid_t y {0};
    for (const auto quadrant : morton_keys::point_insertion_path(morton_keys[point_id]))
    {
        ++depth;
        x <<= 1;
        x += quadrant_x(quadrant);
        y <<= 1;
        y += quadrant_y(quadrant);
        auto child = point_destination->child(quadrant);
        if (not child)
        {
            Box box;
            box.xmin = x * domain.xdim(depth);
            box.ymin = y * domain.ydim(depth);
            box.xmax = (x + 1) * domain.xdim(depth);
            box.ymax = (y + 1) * domain.ydim(depth);
            point_destination->create_child(quadrant, box);
            child = point_destination->child(quadrant);
        }
        point_destination = child;
    }
    point_destination->insert(point_id);
}

inline void initialize_points(
    point_quadtree::Node& root
    , const std::vector<primitives::morton_key_t>& morton_keys
    , const point_quadtree::Domain& domain)
{
    for (primitives::point_id_t i {0}; i < morton_keys.size(); ++i)
    {
        point_quadtree::insert_point(morton_keys, i, root, domain);
    }
}

inline void print_search_pool_sizes(const Tour& tour, const Node& root)
{
    for (primitives::point_id_t i {0}; i < tour.size(); ++i)
    {
        std::vector<primitives::point_id_t> points_next;
        root.get_points(i, tour.search_box_next(i), points_next);
        std::cout << i << ": " << points_next.size();
        std::vector<primitives::point_id_t> points_prev;
        root.get_points(i, tour.search_box_prev(i), points_prev);
        std::cout << ", " << points_prev.size();
        std::cout << std::endl;
    }
}

} // namespace point_quadtree

