#ifndef SHADER_H
#define SHADER_H

#include "rendering/LightData.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <unordered_map>
#include <vector>

struct UniformInfo {
    std::string name;
    GLenum type;
    GLint location;
};

class Shader {
public:
    GLuint programID;
    std::unordered_map<std::string, GLint> locations;

    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    void ApplyLightUniforms(const std::vector<LightData> &lights);
    void Use();
    void SetMat4(const std::string& name, const glm::mat4& value);
    void SetVec3(const std::string& name, const glm::vec3& value);
    void SetFloat(const std::string& name, const float value);
    void SetInt(const std::string& name, const int value);
    void ApplyGlobalUniforms();

    std::vector<UniformInfo> GetCustomUniforms();

private:
    GLuint LoadShader(const std::string& path, GLenum shaderType);
    void CheckShaderCompilation(GLuint shader, const std::string& shaderType);
    void CheckProgramLinking();
    bool IsStandardUniform(const std::string& name) const;
};


#endif
