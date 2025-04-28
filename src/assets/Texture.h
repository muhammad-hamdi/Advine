#pragma once

#include <string>
#include <glad/glad.h> // or GLEW if you're using it
#include <glm/glm.hpp>

namespace Engine {
    class Texture {
    public:
        GLuint ID;
        std::string type;
        std::string path;
        int width, height;

        Texture(const std::string& imagePath, const std::string& type = "diffuse");

        void Bind(GLuint unit = 0) const;
        void UnBind(GLuint unit = 0) const;

    private:
        void LoadFromFile(const std::string& path);
    };
}