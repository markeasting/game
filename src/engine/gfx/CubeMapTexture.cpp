
#include "engine/gfx/CubeMapTexture.h"

#include <iostream>

CubeMapTexture::CubeMapTexture(const std::vector<std::string>& faces) {
    this->loadCubemap(faces);
}

void CubeMapTexture::loadCubemap(const std::vector<std::string>& faces) {
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

    // @TODO run this only once, this is also done in Texture...
    stbi_set_flip_vertically_on_load_thread(1);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        
        if (data) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                0, 
                GL_RGB, 
                width, 
                height, 
                0, 
                GL_RGB, 
                GL_UNSIGNED_BYTE, 
                data
            );
        } else {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
        }

        stbi_image_free(data);
    }

    /* Texture wrapping */
    /* Clamp to edge to prevent black lines at cube edges*/
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    /* Texture filtering: use mipmaps */
    // @TODO check if we need nisotropic filtering / mipmaps / etc
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void CubeMapTexture::bind() const {
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
}
