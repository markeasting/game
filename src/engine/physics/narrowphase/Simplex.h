#pragma once

#include "engine/physics/narrowphase/Support.h"

#include <array>

struct Simplex
{
public:

    bool containsOrigin = false;

    std::array<Support, 4> m_points;
    unsigned m_size;

    Simplex() : m_size(0) {}

    void assign(std::initializer_list<Support> list) {
        for (auto v = list.begin(); v != list.end(); v++) {
            m_points[std::distance(list.begin(), v)] = *v;
        }
        m_size = list.size();
    }

    void push_front(Support point) {
        m_points = {point, m_points[0], m_points[1], m_points[2]};
        m_size = std::min(m_size + 1, 4u);
    }

    Support &operator[](unsigned i) { return m_points[i]; }
    unsigned size() const { return m_size; }

    auto begin() const { return m_points.begin(); }
    auto end() const { return m_points.end() - (4 - m_size); }
};
