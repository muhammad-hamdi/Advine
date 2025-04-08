#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <glad/glad.h> // or GLEW if you're using it
#include <glm/glm.hpp>

class Texture {
public:
    GLuint ID;
    std::string type;
    std::string path;

    Texture(const std::string& imagePath, const std::string& type = "diffuse");

    void Bind(GLuint unit = 0) const;
    void UnBind(GLuint unit = 0) const;

private:
    void LoadFromFile(const std::string& path);
};

#endif
