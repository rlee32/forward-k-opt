#pragma once

// Children are indexed by Morton key quadrant.
// Only leaf nodes have points.

#include "Box.h"
#include <Tour.h>
#include <primitives.h>

#include <array>
#include <iostream>
#include <memory> // unique_ptr
#include <vector>

namespace point_quadtree {

class Node
{
public:
    Node(const Box&);

    void create_child(primitives::quadrant_t, const Box& box);
    const std::array<std::unique_ptr<Node>, 4>& children() const { return m_children; }
    Node* child(primitives::quadrant_t q) { return m_children[q].get(); }

    void insert(primitives::point_id_t i);
    const std::vector<primitives::point_id_t>& points() const { return m_points; }

    std::vector<primitives::point_id_t> find_swap(primitives::point_id_t i
        , const Box& search_box) const;

    void get_points(primitives::point_id_t i
        , const Box& search_box
        , std::vector<primitives::point_id_t>& points) const;

private:
    std::array<std::unique_ptr<Node>, 4> m_children;
    std::vector<primitives::point_id_t> m_points;
    const Box m_box;

    bool touches(const Box&) const;
};

} // namespace point_quadtree
