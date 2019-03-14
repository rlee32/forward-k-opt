#pragma once

#include "primitives.h"

#include <algorithm> // min, max
#include <cmath> // sqrt
#include <unordered_map>
#include <vector>

class LengthMap
{
public:
    LengthMap(const std::vector<primitives::space_t>& x
        , const std::vector<primitives::space_t>& y);

    primitives::length_t length(primitives::point_id_t a, primitives::point_id_t b);

    const std::vector<primitives::space_t>& x() const { return m_x; }
    const std::vector<primitives::space_t>& y() const { return m_y; }

    primitives::space_t x(primitives::point_id_t i) const { return m_x[i]; }
    primitives::space_t y(primitives::point_id_t i) const { return m_y[i]; }

private:
    const std::vector<primitives::space_t>& m_x;
    const std::vector<primitives::space_t>& m_y;
    std::vector<std::unordered_map<primitives::point_id_t, primitives::length_t>> m_lengths;

    primitives::length_t compute_length(primitives::point_id_t a, primitives::point_id_t b) const;
};
