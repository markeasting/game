#pragma once

#include "gfx/Vertex.h"
#include <vector>

struct PrimitiveMesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    PrimitiveMesh() = default;
    PrimitiveMesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices = {});
};
