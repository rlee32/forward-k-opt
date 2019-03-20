#include "Finder.h"

namespace forward {

const std::vector<primitives::point_id_t>& Finder::find_best()
{
    m_best_swap.clear();
    m_best_improvement = 0;
    m_max_search_depth = 0;
    find_forward_swap();
    find_forward_swap_ab();
    return m_best_swap;
}

void Finder::find_forward_swap(const primitives::point_id_t edge_start
    , const primitives::length_t removed_length
    , const primitives::length_t added_length)
{
    std::vector<primitives::point_id_t> points;
    m_root.get_points(edge_start, m_tour.search_box_next(edge_start), points);
    const auto minimum_sequence {m_tour.sequence(edge_start, m_swap_start) + 2};
    const auto remove {m_tour.length(edge_start)};
    for (auto p : points)
    {
        if (m_tour.sequence(p, m_swap_start) < minimum_sequence)
        {
            continue;
        }
        const auto add {m_tour.length(p, edge_start)};
        if (added_length + add >= removed_length + remove)
        {
            continue;
        }
        m_current_swap.push_back(p);
        m_max_search_depth = std::max(m_current_swap.size(), m_max_search_depth);
        const auto new_start {m_tour.prev(p)};
        const auto closing_remove {m_tour.length(new_start)};
        const auto total_remove {removed_length + remove + closing_remove};
        const auto closing_add {m_tour.length(m_swap_end, new_start)};
        const auto total_add {closing_add + added_length + add};
        const bool improving {total_remove > total_add};
        const bool odd_swap_size {(m_current_swap.size() & 1) == 1};
        if (not m_restrict_even or odd_swap_size)
        {
            if (improving)
            {
                check_best(total_remove - total_add);
            }
        }
        find_forward_swap(new_start
            , removed_length + remove
            , added_length + add);
        m_current_swap.pop_back();
    }
}

void Finder::find_forward_swap()
{
    m_restrict_even = false;
    constexpr primitives::point_id_t start {0};
    primitives::point_id_t i {start};
    m_current_swap.clear();
    do
    {
        // option 1
        std::vector<primitives::point_id_t> points;
        m_root.get_points(i, m_tour.search_box_prev(i), points);
        m_swap_start = i;
        m_swap_end = m_tour.prev(i);
        m_current_swap.push_back(i);
        const auto remove {m_tour.prev_length(i)};
        for (auto p : points)
        {
            if (p == i or p == m_tour.prev(i) or p == m_tour.next(i))
            {
                continue;
            }
            const auto add {m_tour.length(p, i)};
            if (add >= remove)
            {
                continue;
            }
            const auto new_start {m_tour.prev(p)};
            m_current_swap.push_back(p);
            const auto next_remove {m_tour.prev_length(p)};
            const auto total_remove {remove + next_remove};
            const auto closing_add {m_tour.length(m_swap_end, new_start)};
            const auto total_add {closing_add + add};
            const auto improving {total_remove > total_add};
            if (improving)
            {
                check_best(total_remove - total_add);
            }
            find_forward_swap(new_start, remove, add);
            m_current_swap.pop_back();
        }
        i = m_tour.next(i);
        m_current_swap.pop_back();
    } while (i != start);
}

// For the first move, the first point in the first edge connects to the next point
//  (as opposed to the second point in the first edge).
// This means that the first move creates a cycle and cannot be closed
//  (e.g. a 2-opt cannot be performed).
void Finder::find_forward_swap_ab()
{
    m_restrict_even = true;
    constexpr primitives::point_id_t start {0};
    primitives::point_id_t i {start};
    m_current_swap.clear();
    do
    {
        // option 2
        std::vector<primitives::point_id_t> points;
        // TODO: search box radius for total gain
        m_root.get_points(i, m_tour.search_box_next(i), points);
        m_swap_start = i;
        m_swap_end = m_tour.next(i);
        m_current_swap.push_back(i);
        const auto remove {m_tour.length(i)};
        for (auto p : points)
        {
            if (p == i or p == m_tour.prev(i) or p == m_tour.next(i))
            {
                continue;
            }
            const auto add {m_tour.length(p, i)};
            if (add >= remove)
            {
                continue;
            }
            m_current_swap.push_back(p);
            const auto new_start {m_tour.prev(p)};
            find_forward_swap(new_start, remove, add);
            m_current_swap.pop_back();
        }
        m_current_swap.pop_back();
        i = m_tour.next(i);
    } while (i != start);
}


} // namespace forward

