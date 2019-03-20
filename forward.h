#pragma once

#include "Tour.h"
#include "constants.h"
#include "point_quadtree/Node.h"
#include "primitives.h"

namespace forward {

// Returns true if "swap" is improving.
inline bool find_forward_swap(Tour& tour
    , const point_quadtree::Node& root
    , const primitives::point_id_t global_swap_start
    , const primitives::point_id_t global_swap_end
    , const primitives::point_id_t edge_start
    , const primitives::length_t removed_length
    , const primitives::length_t added_length
    , std::vector<primitives::point_id_t>& swap
    , bool restrict_even_removals = false)
{
    std::vector<primitives::point_id_t> points;
    root.get_points(edge_start, tour.search_box_next(edge_start), points);
    const auto minimum_sequence {tour.sequence(edge_start, global_swap_start) + 2};
    for (auto p : points)
    {
        if (tour.sequence(p, global_swap_start) < minimum_sequence)
        {
            continue;
        }
        const auto addition {tour.length(p, edge_start)};
        const auto removal {tour.length(edge_start)};
        if (addition >= removal)
        {
            continue;
        }
        swap.push_back(p);
        const auto new_start {tour.prev(p)};
        const auto closing_length {tour.length(global_swap_end, new_start)};
        const bool improving {removed_length + removal + tour.length(new_start)
            > closing_length + added_length + addition};
        const bool odd_swap_size {(swap.size() & 1) == 1};
        if (not restrict_even_removals or odd_swap_size)
        {
            if (improving)
            {
                return true;
            }
        }
        if (find_forward_swap(tour, root, global_swap_start, global_swap_end
            , new_start
            , removed_length + removal
            , added_length + addition
            , swap
            , restrict_even_removals))
        {
            return true;
        }
        swap.pop_back();
    }
    return false;
}

inline std::vector<primitives::point_id_t> find_forward_swap(Tour& tour
    , const point_quadtree::Node& root)
{
    // for each point p in swap vector, edge (p, prev(p)) is deleted.
    constexpr primitives::point_id_t start {0};
    primitives::point_id_t i {start};
    do
    {
        // If the "first" (direction of traversal) point of any edge is "a"
        //  and the second point is "b", then the first move can be made of either:
        // 1. first destroyed edge b to next destroyed edge b
        // 2. first destroyed edge a to next destroyed edge b
        // In option 2, even-numbered edge removals cannot result in a single cycle,
        //  e.g. 2-opt, 4-opt, 6-opt, etc. cannot be performed without splitting the
        //  tour into 2 cycles.
        // In a forward swap, all subsequent destroyed edges must be
        //  downstream / later in the tour, and all moves (except the first)
        //  must go from a to b (of the next destroyed edge).

        // option 1
        std::vector<primitives::point_id_t> points;
        root.get_points(i, tour.search_box_prev(i), points);
        const auto global_swap_end {tour.prev(i)};
        for (auto p : points)
        {
            if (p == i or p == tour.prev(i) or p == tour.next(i))
            {
                continue;
            }
            const auto addition {tour.length(p, i)};
            const auto deletion {tour.prev_length(i)};
            if (addition >= deletion)
            {
                continue;
            }
            const auto new_start {tour.prev(p)};
            const auto next_deletion {tour.prev_length(p)};
            const auto closing_length {tour.length(global_swap_end, new_start)};
            std::vector<primitives::point_id_t> swap {i, p};
            if (closing_length + addition < deletion + next_deletion)
            {
                return swap; // 2-opt.
            }
            if (find_forward_swap(tour, root, global_swap_end, global_swap_end
                , new_start
                , deletion
                , addition
                , swap))
            {
                return swap;
            }
        }
        i = tour.next(i);
    } while (i != start);
    return {};
}

// For the first move, the first point in the first edge connects to the next point
//  (as opposed to the second point in the first edge).
// This means that the first move creates a cycle and cannot be closed
//  (e.g. a 2-opt cannot be performed).
inline std::vector<primitives::point_id_t> find_forward_swap_ab(Tour& tour
    , const point_quadtree::Node& root)
{
    // for each point p in swap vector, edge (p, prev(p)) is deleted.
    constexpr primitives::point_id_t start {0};
    primitives::point_id_t i {start};
    do
    {
        // If the "first" (direction of traversal) point of any edge is "a"
        //  and the second point is "b", then the first move can be made of either:
        // 1. first destroyed edge b to next destroyed edge b
        // 2. first destroyed edge a to next destroyed edge b
        // In option 2, even-numbered edge removals cannot result in a single cycle,
        //  e.g. 2-opt, 4-opt, 6-opt, etc. cannot be performed without splitting the
        //  tour into 2 cycles.
        // In a forward swap, all subsequent destroyed edges must be
        //  downstream / later in the tour, and all moves (except the first)
        //  must go from a to b (of the next destroyed edge).

        // option 2
        std::vector<primitives::point_id_t> points;
        root.get_points(i, tour.search_box_next(i), points);
        for (auto p : points)
        {
            if (p == i or p == tour.prev(i) or p == tour.next(i))
            {
                continue;
            }
            const auto addition {tour.length(p, i)};
            const auto deletion {tour.length(i)};
            if (addition >= deletion)
            {
                continue;
            }
            const auto new_start {tour.prev(p)};
            std::vector<primitives::point_id_t> swap {i, p};
            const auto global_swap_start {i};
            const auto global_swap_end {tour.next(i)};
            if (find_forward_swap(tour, root, global_swap_start, global_swap_end
                , new_start
                , deletion
                , addition
                , swap
                , true))
            {
                return swap;
            }
        }
        i = tour.next(i);
    } while (i != start);
    return {};
}

} // namespace forward
