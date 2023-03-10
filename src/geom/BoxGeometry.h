#pragma once

#include "geom/Geometry.h"

class BoxGeometry : public Geometry {
public:

    BoxGeometry(const float& size = 1.0f);
    BoxGeometry(const float& width, const float& height, const float& depth);

private:
    void generate(const float& width, const float& height, const float& depth);
};
