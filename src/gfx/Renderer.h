#pragma once

#include "common.h"
#include "gfx/Shader.h"
#include "gfx/Uniforms.h"
#include "gfx/Mesh.h"
#include "camera/Camera.h"

class Renderer {
public:

	Renderer();
	Renderer(const Renderer&) = delete;
	~Renderer();
	
	void setSize(int width, int height);
	
	void add(Ref<Mesh> mesh); // @TODO move to scene!

	void draw(Camera* camera);
	void clear();

private:
	std::vector<Ref<Mesh>> m_meshes = {};

	bool m_useRenderpass = true;
	Mesh m_fullscreenQuad;

    Material m_defaultShader = { "Basic" };
    Material m_screenShader = { "Basic.vert", "Renderpass.frag" };
	
};

static void GlDebugMsg(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
