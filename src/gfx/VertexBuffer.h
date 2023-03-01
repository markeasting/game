#pragma once

#include "common.h"
#include "gfx/Vertex.h"

#include <vector>

/**
 * @TODO change `unsigned int` to a common type (also update in mesh / geometry)
 */
class VertexBuffer {
public:
	GLuint vao = 0;
	GLuint vbuffer = 0;
	std::vector<Vertex> vertices;

	VertexBuffer();
	VertexBuffer(std::vector<Vertex> vertices, int mode = GL_STATIC_DRAW);
	~VertexBuffer();

	void setVertices(std::vector<Vertex> vertices, int mode = GL_STATIC_DRAW);
	unsigned int getCount() const;
	unsigned int getSize() const;
	void bind() const;
	void unbind() const;

private:
	unsigned int count = 0;
	unsigned int size = 0;
};
