#pragma once

#include "engine/common/ref.h"
#include "engine/common/gl.h"

#include "engine/gfx/Mesh.h"

#include "engine/scene/Scene.h"
#include "engine/core/Camera.h"

struct RendererConfig {
	bool wireframe = false;
	bool useRenderpass = true;
};

class Renderer {
public:

 	RendererConfig m_config;

	Renderer(RendererConfig config = {});

	Renderer(const Renderer&) = delete;
	~Renderer();

	void setSize(int width, int height);

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
