#pragma once

#include "Tour.h"
#include "constants.h"
#include "point_quadtree/Node.h"
#include "primitives.h"

namespace solver {

inline std::vector<primitives::point_id_t> find_forward_swap(Tour& tour
    , const point_quadtree::Node& root)
{
    constexpr primitives::point_id_t start {0};
    primitives::point_id_t i {start};
    do
    {
        // If the "first" (direction of traversal) point of any edge is "a"
        //  and the second point is "b", then the first move can be made of either:
        // 1. first destroyed edge b to next destroyed edge b
        // 2. first destroyed edge a to next destroyed edge b
        // In option 2, the tour cannot be closed as a valid tour until the next move.
        // In a forward swap, all subsequent destroyed edges must be
        //  downstream / later in the tour, and all moves (except the first)
        //  must go from a to b (of the next destroyed edge).

        // option 1
        std::vector<primitives::point_id_t> points;
        root.get_points(i, tour.search_box_prev(i), points);
        auto end {tour.prev(i)};
        for (auto p : points)
        {
            if (p == i or p == tour.prev(i) or p == tour.next(i))
            {
                continue;
            }
            auto addition {tour.length(p, i)};
            auto deletion {tour.prev_length(i)};
            if (addition >= deletion)
            {
                continue;
            }
            const auto new_point {tour.prev(p)};
            deletion += tour.prev_length(p);
            const auto closing_length {tour.length(end, new_point)};
            if (closing_length + addition < deletion)
            {
                // 2-opt.
                return {i, p};
            }
        }

        // option 2
        points.clear();
        root.get_points(i, tour.search_box_next(i), points);
        end = tour.next(i);
        for (auto p : points)
        {
            if (p == i or p == tour.prev(i) or p == tour.next(i))
            {
                continue;
            }
            auto new_length {tour.length(p, i)};
            if (new_length >= tour.length(i))
            {
                continue;
            }
        }
        i = tour.next(i);
    } while (i != start);
    return {};
}

} // namespace solver
