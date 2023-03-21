#pragma once

#include "common.h"


class FrameBuffer {
public:

    FrameBuffer();

    void create(float width, float height);
    void invalidate();

    void bind() const;

    GLuint getTexture() const;

private:
	GLuint m_fbo = 0;
	GLuint m_rbo = 0;

    GLuint m_textureColorbuffer = 0;

};
