#pragma once

#include "common.h"

#include "gfx/Shader.h"
#include "gfx/Uniforms.h"
#include "gfx/Mesh.h"

#include "scene/Scene.h"
#include "camera/Camera.h"

struct RendererConfig {
	bool wireframe = false;
	bool useRenderpass = true;
};

class Renderer {
public:

 	RendererConfig m_config;

	Renderer();
	Renderer(const Renderer&) = delete;
	~Renderer();
	
	void setSize(int width, int height);
		
	// @TODO should be a weak ptr?
	// void add(Ref<Mesh> mesh);

	void draw(Ref<Scene> scene, Ref<Camera> camera);
	void drawMesh(Ref<Mesh> mesh, Ref<Camera> camera);
	void clear();

private:

	/**
	 * @todo make FullSceenQuad class for other render passes
	 */
	Mesh m_fullscreenQuad;

	// std::vector<Ref<Mesh>> m_meshes = {};
    // Material m_defaultShader = { "Basic" };
    Material m_screenShader = { "Basic.vert", "Renderpass.frag" };
	
};

static void GlDebugMsg(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
