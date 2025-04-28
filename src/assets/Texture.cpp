#include "Texture.h"
#include <stb_image.h>
#include <iostream>

namespace Engine {
    Texture::Texture(const std::string& imagePath, const std::string& type)
        : type(type), path(imagePath) {
        LoadFromFile(imagePath);
    }

    void Texture::LoadFromFile(const std::string& path) {
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        // Texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Load image
        int nrChannels;
        stbi_set_flip_vertically_on_load(true); // Flip to match OpenGL convention
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

        if (data) {
            GLenum format = GL_RGB;
            if (nrChannels == 1) format = GL_RED;
            else if (nrChannels == 4) format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cerr << "Failed to load texture: " << path << std::endl;
        }
        stbi_image_free(data);
    }

    void Texture::Bind(GLuint unit) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, ID);
    }

    void Texture::UnBind(GLuint unit) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}