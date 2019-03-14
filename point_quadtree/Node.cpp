#include "Node.h"

namespace point_quadtree {

Node::Node(const Box& box) : m_box(box) {}

void Node::insert(primitives::point_id_t i)
{
    m_points.push_back(i);
}

void Node::create_child(primitives::quadrant_t quadrant, const Box& box)
{
    if (m_children[quadrant])
    {
        return;
    }
    m_children[quadrant] = std::make_unique<Node>(box);
}

bool Node::touches(const Box& box) const
{
    return m_box.touches(box);
}

void Node::get_points(primitives::point_id_t i
    , const Box& search_box
    , std::vector<primitives::point_id_t>& points) const
{
    if (m_points.empty())
    {
        for (const auto& unique_ptr : m_children)
        {
            if (unique_ptr and unique_ptr->touches(search_box))
            {
                unique_ptr->get_points(i, search_box, points);
            }
        }
    }
    else
    {
        for (auto p : m_points)
        {
            points.push_back(p);
        }
    }
}

} // namespace point_quadtree
