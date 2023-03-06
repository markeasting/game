#pragma once

#include "common_includes.h"
// #include "util/Filesystem.h"
#include "gfx/Shader.h"
#include "gfx/Uniforms.h"
#include "geom/Mesh.h"
#include "camera/Camera.h"
// #include "window/WindowEventHandler.h"

class Renderer {
public:
    Material m_defaultShader = { "/shader/Basic" };

	uint16_t m_frameBufferWidth = 0;
	uint16_t m_frameBufferHeight = 0;

	Renderer();
	Renderer(const Renderer&) = delete;
	~Renderer();
	
	void setupFramebuffer(const int& width, const int& height);
	
	void add(Ref<Mesh> mesh); // @TODO move to scene!

	void draw(Camera* camera);
	void clear();

private:
	std::vector<Ref<Mesh>> m_meshes = {};
	
};

static void GlDebugMsg(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
