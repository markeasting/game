#include "gfx/FrameBuffer.h"

/**
 * https://learnopengl.com/Advanced-OpenGL/Framebuffers 
 */
FrameBuffer::FrameBuffer() {

}

void FrameBuffer::create(float width, float height) {

    this->invalidate();

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    /* Generate texture */
    glGenTextures(1, &m_textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // float borderColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Attach it to currently bound framebuffer object */
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorbuffer, 0);  

    /* Create render buffer object */
    glGenRenderbuffers(1, &m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);  
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    /* Attach the renderbuffer object to the depth and stencil attachment of the framebuffer */
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Log("Framebuffer is not complete!", LogLevel::ERROR);

    /* Unbind the created framebuffer (rebind default 0) to make sure we're not using the wrong framebuffer */
    /* If this results in a blank screen, it could be that the default framebuffer has a diffent ID! */
    /* In that case, get the default ID by calling glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO); */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
}

void FrameBuffer::invalidate() {
    if (m_fbo != 0) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);   
        glDeleteFramebuffers(1, &m_fbo);
    }
}

void FrameBuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    /* Set new viewport if you want to render to a specific part of the screen */
    // glViewport(w, h)
}

GLuint FrameBuffer::getTexture() const {
    return m_textureColorbuffer;
}
