
#include "gfx/Renderer.h"

#include "camera/Camera.h"

Renderer::Renderer() {

    if (GLVersion.major > 4 && GLVersion.minor >= 3) {
        glDebugMessageCallback(GlDebugMsg, nullptr);
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // background color

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Wireframe GL_LINE ->> https://vitaliburkov.wordpress.com/2016/09/17/simple-and-fast-high-quality-antialiased-lines-with-opengl/
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
    // https://learnopengl.com/Advanced-OpenGL/Stencil-testing
    // glEnable(GL_STENCIL_TEST);
    // glStencilMask(0xFF); // Enable writing to the stencil buffer
	// glStencilFunc(GL_EQUAL, 1, 0xFF);
	// glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); // Default, see docs for custom behavior

    std::vector<Vertex> geometry {
        Vertex(vec3(  1, -1, 0 ), vec3( 0, 0, 1 ), vec2( 1, 0 )),
        Vertex(vec3(  1,  1, 0 ), vec3( 0, 0, 1 ), vec2( 1, 1 )),
        Vertex(vec3( -1, -1, 0 ), vec3( 0, 0, 1 ), vec2( 0, 0 )),
        Vertex(vec3( -1,  1, 0 ), vec3( 0, 0, 1 ), vec2( 0, 1 )),
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 1, 3,
    };

    m_fullscreenQuad.m_geometry = ref<Geometry>(geometry, indices);

    // @TODO move uniform settings to Shader? Then we don't need a material here
    m_fullscreenQuad.setMaterial(m_screenShader);

}

void Renderer::setSize(int width, int height) {
    
    glViewport(0, 0, width, height);

}

// @TODO move to scene!
// void Renderer::add(Ref<Mesh> mesh) {

//     if (mesh->m_material == nullptr)
//         mesh->setMaterial(m_defaultShader);

//     if (mesh->m_material->m_shader->m_program == 0) {
//         Log("Shader failed to compile, using default shader");
//         mesh->setMaterial(m_defaultShader);
//     }

//     m_meshes.push_back(mesh);
// }

void Renderer::draw(Ref<Scene> scene, Ref<Camera> camera) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // if(g_settings.wireframe) {
    //     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //     glDisable(GL_CULL_FACE);
    // } else {
    //     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //     glEnable(GL_CULL_FACE);
    // }

    if (m_useRenderpass) {
        camera->m_frameBuffer.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
        glEnable(GL_DEPTH_TEST);
    }
    
    for (auto const& pair : scene->m_layers.all()) {
        
        auto& layer = pair.second;

        if (!layer->m_active)
            continue;

        /* Draw opaque meshes */
        for (auto const& mesh : layer->m_meshes) {

            if (mesh->m_material->transparent)
                continue;

            this->drawMesh(mesh, camera);

            // mesh->unbind(); // @TODO check if unbinding VAO / shader / texture is required
        }

        /* Draw transparent meshes */
        for (auto const& mesh : layer->m_meshes) {

            if (!mesh->m_material->transparent)
                continue;

            this->drawMesh(mesh, camera);

            // mesh->unbind(); // @TODO check if unbinding VAO / shader / texture is required
        }
    }

    if (m_useRenderpass) {
    
        /* Final render pass */
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_CULL_FACE);

        m_fullscreenQuad.bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, camera->m_frameBuffer.getTexture());

        glDrawElements(GL_TRIANGLES, m_fullscreenQuad.m_geometry->m_indexBuffer->getCount(), GL_UNSIGNED_INT, 0);
    }

}

void Renderer::drawMesh(Ref<Mesh> mesh, Ref<Camera> camera) {

    mesh->bind();

    auto matrix = mesh->getWorldPositionMatrix();

    mesh->m_material->setUniform(
        "u_modelViewMatrix", 
        mesh->m_useProjectionMatrix 
            ? camera->m_viewMatrix * matrix
            : matrix
    );

    mesh->m_material->setUniform(
        "u_modelViewProjectionMatrix", 
        mesh->m_useProjectionMatrix 
            ? camera->m_viewProjectionMatrix * matrix
            : matrix
    );

    if(mesh->m_material && mesh->m_material->wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_CULL_FACE);
    }

    // @TODO add support for instanced meshes using glDrawArraysInstanced and glDrawElementsInstanced
    if(mesh->m_geometry->hasIndices()) {
        glDrawElements(GL_TRIANGLES, mesh->m_geometry->m_indexBuffer->getCount(), GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, mesh->m_geometry->m_vertexBuffer->getCount());
    }
}

void Renderer::clear() {
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

Renderer::~Renderer() {
    
}

static void GlDebugMsg(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
    switch(severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            Log(message, LogLevel::ERROR);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            Log(message, LogLevel::ERROR);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            Log(message, LogLevel::ERROR);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            // Log(message, LogLevel::DEBUG);
            break;
    }
}
