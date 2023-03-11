#pragma once

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "util/stb_image.h"

#include "common.h"

class Texture {
public:

    Texture();

    void load(const char* source);
    void bind() const;

private:

    GLuint m_texture = 0;

};
