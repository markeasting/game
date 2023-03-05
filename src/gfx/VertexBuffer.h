#pragma once

#include "common_includes.h"
#include "gfx/Vertex.h"

#include <vector>

/**
 * @TODO merge with IndexBuffer, extend base class
 * @TODO change `unsigned int` to a common type (also update in mesh / geometry)
 */
class VertexBuffer {
public:
	std::vector<Vertex> m_vertices;

	VertexBuffer();
	VertexBuffer(std::vector<Vertex> vertices, int mode = GL_STATIC_DRAW);
	~VertexBuffer();

	void setVertices(std::vector<Vertex> vertices, int mode = GL_STATIC_DRAW);
	unsigned int getCount() const;
	unsigned int getSize() const;
	void bind() const;
	void unbind() const;

private:
	GLuint m_vao = 0;
	GLuint m_buffer = 0;

	unsigned int m_count = 0;
	unsigned int m_size = 0;
};
