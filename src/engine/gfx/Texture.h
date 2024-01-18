#pragma once

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "engine/common/gl.h"

class Texture {
public:

    Texture();

    void load(const char* source);
    virtual void bind() const;

// private:

    GLuint m_texture = 0;
    GLenum m_format = GL_RGB;

    static constexpr const char* defaultTexture = "assets/texture/default.jpg";

};
