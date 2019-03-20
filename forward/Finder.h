#pragma once

// Finds best-improvement forward swap.
// Does not modify tour, but Tour has length computer that records queried lengths.

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

#include <point_quadtree/Node.h>
#include <Tour.h>
#include <primitives.h>

#include <vector>

namespace forward {

class Finder
{
public:
    Finder(const point_quadtree::Node& root, Tour& tour) : m_root(root), m_tour(tour) {}

    const std::vector<primitives::point_id_t>& find_best();
    const std::vector<primitives::point_id_t>& best() const { return m_best_swap; }
    bool restrict_even_best() const { return m_restrict_even_best; }
    size_t max_search_depth() const { return m_max_search_depth; }

private:
    const point_quadtree::Node& m_root;
    Tour& m_tour;

    // for each point p in swap vector, edge (p, prev(p)) is deleted.
    std::vector<primitives::point_id_t> m_current_swap;
    std::vector<primitives::point_id_t> m_best_swap;
    primitives::length_t m_best_improvement {0};
    bool m_restrict_even_best {false};
    size_t m_max_search_depth {0};

    primitives::point_id_t m_swap_start {constants::invalid_point};
    primitives::point_id_t m_swap_end {constants::invalid_point};
    // If the first move is a to b, even-numbered k-opt moves will split the tour.
    // if true, even-numbered k-opt moves are prohibited from m_swap.
    bool m_restrict_even {false};

    void find_forward_swap(const primitives::point_id_t edge_start
        , const primitives::length_t removed_length
        , const primitives::length_t added_length);
    void find_forward_swap();
    void find_forward_swap_ab();
    void check_best(primitives::length_t improvement)
    {
        if (improvement > m_best_improvement)
        {
            m_best_swap = m_current_swap;
            m_best_improvement = improvement;
            m_restrict_even_best = m_restrict_even;
        }
    }
};

} // namespace forward

