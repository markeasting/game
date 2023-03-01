#pragma once

#include "common.h"
#include "gfx/Vertex.h"

#include <vector>

/**
 * @TODO change `unsigned int` to a common type (also update in mesh / geometry)
 */
class IndexBuffer {
public:
	GLuint ibuffer = 0;
	std::vector<unsigned int> indices;
	
	IndexBuffer();
	IndexBuffer(std::vector<unsigned int> indices, int mode = GL_STATIC_DRAW);
	~IndexBuffer();

	void setIndices(std::vector<unsigned int> indices, int mode = GL_STATIC_DRAW);
	unsigned int getCount() const;
	unsigned int getSize() const;
	void bind() const;
	void unbind() const;

private:
	unsigned int count = 0;
	unsigned int size = 0;
};
