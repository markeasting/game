#pragma once

#include "common_includes.h"
#include "gfx/Vertex.h"

#include <vector>

struct Polygon {
    std::vector<Vertex> vertices;
    Polygon(const std::vector<Vertex> &vertices) : vertices(vertices) {};
    ~Polygon() = default;
};
