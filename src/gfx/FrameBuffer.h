#pragma once

#include "common_includes.h"

class FrameBuffer {
public:

    FrameBuffer();

    void create(const float& width, const float& height);
    void invalidate();

    void bind() const;

    GLuint getTexture() const;

private:
	GLuint m_fbo = 0;
	GLuint m_rbo = 0;

    GLuint m_textureColorbuffer = 0;

};
