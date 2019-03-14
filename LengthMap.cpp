#include "LengthMap.h"

LengthMap::LengthMap(const std::vector<primitives::space_t>& x
    , const std::vector<primitives::space_t>& y)
    : m_x(x), m_y(y), m_lengths(x.size()) {}

primitives::length_t LengthMap::compute_length(
    primitives::point_id_t a, primitives::point_id_t b) const
{
    auto dx = m_x[a] - m_x[b];
    auto dy = m_y[a] - m_y[b];
    auto exact = std::sqrt(dx * dx + dy * dy);
    return exact + 0.5; // return type cast.
}

primitives::length_t LengthMap::length(primitives::point_id_t a, primitives::point_id_t b)
{
    auto min {std::min(a, b)};
    auto& lengths {m_lengths[min]};
    auto max {std::max(a, b)};
    auto it {lengths.find(max)};
    if (it == std::cend(lengths))
    {
        auto length {compute_length(min, max)};
        lengths[max] = length;
        return length;
    }
    return it->second;
}

