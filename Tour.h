#pragma once

#include "LengthMap.h"
#include "constants.h"
#include "point_quadtree/Box.h"
#include "point_quadtree/Domain.h"
#include "primitives.h"

#include <array>
#include <cstdlib> // abort
#include <iostream>
#include <vector>

class Tour {
    using Adjacents = std::array<primitives::point_id_t, 2>;
public:
    Tour(const std::vector<primitives::point_id_t>& initial_tour, LengthMap*);

    void forward_swap(const std::vector<primitives::point_id_t> swap, bool cyclic_first);
    void move(primitives::point_id_t a, primitives::point_id_t b);
    void vmove(primitives::point_id_t v, primitives::point_id_t n);
    primitives::point_id_t next(primitives::point_id_t i) const { return m_next[i]; }
    primitives::point_id_t prev(primitives::point_id_t i) const;
    std::vector<primitives::point_id_t> order() const;
    primitives::point_id_t size() const { return m_next.size(); }

    primitives::point_id_t sequence(primitives::point_id_t i, primitives::point_id_t start) const;

    primitives::space_t x(primitives::point_id_t i) const { return m_length_map->x(i); }
    primitives::space_t y(primitives::point_id_t i) const { return m_length_map->y(i); }

    primitives::length_t length() const;
    primitives::length_t length(primitives::point_id_t i) const;
    primitives::length_t prev_length(primitives::point_id_t i) const;
    primitives::length_t length(primitives::point_id_t i, primitives::point_id_t j)
    {
        return m_length_map->length(i, j);
    }

    const LengthMap& length_map() const { return *m_length_map; }

    Box search_box_next(primitives::point_id_t i) const;
    Box search_box_prev(primitives::point_id_t i) const;
    Box search_box(primitives::point_id_t i, primitives::length_t radius) const;

    void validate() const;

private:
    LengthMap* m_length_map {nullptr};
    std::vector<Adjacents> m_adjacents;
    std::vector<primitives::point_id_t> m_next;
    std::vector<primitives::point_id_t> m_sequence;

    void reset_adjacencies(const std::vector<primitives::point_id_t>& initial_tour);
    void update_next();

    primitives::point_id_t get_other(primitives::point_id_t point, primitives::point_id_t adjacent) const;
    void create_adjacency(primitives::point_id_t point1, primitives::point_id_t point2);
    void fill_adjacent(primitives::point_id_t point, primitives::point_id_t new_adjacent);
    void break_adjacency(primitives::point_id_t i);
    void break_adjacency(primitives::point_id_t point1, primitives::point_id_t point2);
    void vacate_adjacent_slot(primitives::point_id_t point, primitives::point_id_t adjacent, int slot);
};

