
#include "gfx/Renderer.h"

#include "camera/Camera.h"

Renderer::Renderer() {

    // m_windowWidth = 1280; //g_settings.window_width;
    // m_windowHeight = 720; //g_settings.window_height;
    
    // glfwInit();

    // #ifdef __APPLE__
    //     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    //     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    //     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // #endif
    
    // // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // Disables border / close button etc. 
    // // glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
    // // glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_FALSE);
    // glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    // m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "Hello World!", NULL, NULL);

    // if (!m_window) {
    //     Log("GLFW window could not be created.", LogLevel::ERROR);
    //     glfwTerminate();
    // }

    // glfwMakeContextCurrent(m_window);

    // // if(g_settings.vsync)
	//     glfwSwapInterval(1);
    // // else 
	// //     glfwSwapInterval(0);
    
    // if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    //     Log("Failed to initialize OpenGL context.", LogLevel::ERROR);
    //     glfwTerminate();
    // }

    // printf("Initialized with OpenGL %d.%d.\n", GLVersion.major, GLVersion.minor);
    // printf("Supported GLSL version is %s.\n", (char*) glGetString(GL_SHADING_LANGUAGE_VERSION));





    // camera = new Camera(m_window); // Requires the framebuffer to be set up.
    // setupFramebuffer(m_frameBufferWidth, m_frameBufferHeight);




    // Breaks on MacOS?
    glDebugMessageCallback(GlDebugMsg, nullptr);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);



    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // background color

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Wireframe ->> https://vitaliburkov.wordpress.com/2016/09/17/simple-and-fast-high-quality-antialiased-lines-with-opengl/
	// glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
    // glEnable(GL_STENCIL_TEST);
	// glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	// glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    m_defaultShader = new Material("/shader/Basic");
    
    // WindowEventHandler::init(m_window, true);
    // WindowEventHandler::onFrame(m_window);

}

void Renderer::setupFramebuffer(const int& width, const int& height) {

    m_frameBufferWidth = width;
    m_frameBufferHeight = height;

    // if (width == 0 || height == 0) {
    //     glfwGetFramebufferSize(m_window, &m_frameBufferWidth, &m_frameBufferHeight);
    // }
    
    glViewport(0, 0, m_frameBufferWidth, m_frameBufferHeight);

}

// @TODO move to scene!
void Renderer::add(Mesh* mesh) {
    m_meshes.push_back(mesh);
}

void Renderer::draw(Camera* camera) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // if(g_settings.wireframe) {
    //     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //     glDisable(GL_CULL_FACE);
    // } else {
    //     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //     glEnable(GL_CULL_FACE);
    // }

    for (auto& mesh : m_meshes) {

        mesh->bind();
        
        glUniformMatrix4fv(
            mesh->m_material->m_shader->getUniformLocation("u_modelViewMatrix"), 
            1,
            GL_FALSE, 
            glm::value_ptr(camera->viewMatrix * mesh->getWorldPositionMatrix())
        );
        glUniformMatrix4fv(
            mesh->m_material->m_shader->getUniformLocation("u_modelViewProjectionMatrix"),
            1, 
            GL_FALSE, 
            glm::value_ptr(camera->viewProjectionMatrix * mesh->getWorldPositionMatrix())
        );

        if(mesh->m_indexBuffer->getCount() > 0) {
            glDrawElements(GL_TRIANGLES, mesh->m_indexBuffer->getCount(), GL_UNSIGNED_INT, 0);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, mesh->m_vertexBuffer->getCount());
        }

        // @TODO add support for instanced meshes using glDrawArraysInstanced and glDrawElementsInstanced
    }
}

void Renderer::clear() {
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Renderer::~Renderer() {
    for (auto& mesh: m_meshes) {
        delete mesh;
    }

    glfwTerminate();

    Log("[RENDERER] MOIIIIIII\n", LogLevel::DEBUG);
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
