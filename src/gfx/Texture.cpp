#include "gfx/Texture.h"

Texture::Texture() {

}

void Texture::load(const char* source) {

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load_thread(1);
    unsigned char *data = stbi_load(source, &width, &height, &nrChannels, 0); 

    if (!data) {
        // @TODO set default texture using glBindTexture(GL_TEXTURE_2D, 0) ?
        data = stbi_load(Texture::defaultTexture, &width, &height, &nrChannels, 0); 
        Log("Failed to load texture, using default texture");
    }

    if (!data) {
        Log("Failed to load texture");
        std::exit(EXIT_FAILURE);
    }

    /* Upload data and generate mips */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    /* Texture settings */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    stbi_image_free(data);

}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, m_texture);
}
