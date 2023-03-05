#pragma once

#include "common_includes.h"
#include "gfx/Vertex.h"

#include <vector>

/**
 * @TODO merge with VertexBuffer, extend base class
 * @TODO change `unsigned int` to a common type (also update in mesh / geometry)
 */
class IndexBuffer {
public:
	std::vector<unsigned int> m_indices;
	
	IndexBuffer();
	IndexBuffer(std::vector<unsigned int> indices, int mode = GL_STATIC_DRAW);
	~IndexBuffer();

	void setIndices(std::vector<unsigned int> indices, int mode = GL_STATIC_DRAW);
	unsigned int getCount() const;
	unsigned int getSize() const;
	void bind() const;
	void unbind() const;

private:
	GLuint m_buffer = 0;
	
	unsigned int m_count = 0;
	unsigned int m_size = 0;
};
