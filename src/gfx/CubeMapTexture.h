#pragma once

#include "common.h"
#include "gfx/Texture.h"

class CubeMapTexture : public Texture {
public: 

    CubeMapTexture() = default;
    CubeMapTexture(const std::vector<std::string>& faces);

    void loadCubemap(const std::vector<std::string>& faces);

    void bind() const;

};
