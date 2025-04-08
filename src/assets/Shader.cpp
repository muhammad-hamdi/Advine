#include "Shader.h"

#include "rendering/Renderer.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
    // Load shaders
    GLuint vertexShader = LoadShader(vertexShaderPath, GL_VERTEX_SHADER);
    GLuint fragmentShader = LoadShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

    // Create program
    programID = glCreateProgram();

    // Attach shaders
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);

    // Link program
    glLinkProgram(programID);
    CheckProgramLinking();

    // Clean up shaders as they're no longer needed after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

GLuint Shader::LoadShader(const std::string& path, GLenum shaderType) {
    // Read shader file
    std::ifstream shaderFile(path);
    if (!shaderFile.is_open()) {
        std::cerr << "Failed to open shader file: " << path << std::endl;
        return 0;
    }
    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    std::string shaderCode = shaderStream.str();

    // Compile shader
    GLuint shader = glCreateShader(shaderType);
    const char* shaderCodeCStr = shaderCode.c_str();
    glShaderSource(shader, 1, &shaderCodeCStr, nullptr);
    glCompileShader(shader);
    CheckShaderCompilation(shader, shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment");

    return shader;
}

void Shader::CheckShaderCompilation(GLuint shader, const std::string& shaderType) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << shaderType << " shader compilation failed:\n" << infoLog << std::endl;
    }
}

void Shader::CheckProgramLinking() {
    GLint success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        std::cerr << "Program linking failed:\n" << infoLog << std::endl;
    }
}

bool Shader::IsStandardUniform(const std::string &name) const
{
    return name == "model" || name == "view" || name == "projection";
}

void Shader::ApplyGlobalUniforms()
{
    SetMat4("u_View", Renderer::GetViewMatrix());
    SetMat4("u_Projection", Renderer::GetProjectionMatrix());
    SetVec3("u_CameraPos", Renderer::GetCameraPosition());
}


void Shader::ApplyLightUniforms(const std::vector<LightData>& lights) {
    for (int i = 0; i < lights.size(); ++i) {
        const auto& light = lights[i];
        std::string prefix = "u_Lights[" + std::to_string(i) + "]";

        SetInt(prefix + ".type", light.type);
        SetVec3(prefix + ".color", light.color);
        SetVec3(prefix + ".position", light.position);
        SetFloat(prefix + ".range", light.range);
        SetVec3(prefix + ".direction", light.direction);
        SetFloat(prefix + ".spotAngle", light.spotAngle);
        SetInt(prefix + ".castShadows", light.castShadows);
    }

    SetInt("u_LightCount", static_cast<int>(lights.size()));
}

void Shader::Use() {
    glUseProgram(programID);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) {
    GLint loc;
    if(locations.find(name) != locations.end()) {
        loc = locations[name];
    } else {
        loc = glGetUniformLocation(programID, name.c_str());
    }
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetVec3(const std::string &name, const glm::vec3 &value)
{
    GLint loc;
    if(locations.find(name) != locations.end()) {
        loc = locations[name];
    } else {
        loc = glGetUniformLocation(programID, name.c_str());
    }
    glUniform3fv(loc, 1, glm::value_ptr(value));
}

void Shader::SetFloat(const std::string &name, const float value)
{
    GLint loc;
    if(locations.find(name) != locations.end()) {
        loc = locations[name];
    } else {
        loc = glGetUniformLocation(programID, name.c_str());
    }
    glUniform1f(loc, value);
}

void Shader::SetInt(const std::string &name, const int value)
{
    GLint loc;
    if(locations.find(name) != locations.end()) {
        loc = locations[name];
    } else {
        loc = glGetUniformLocation(programID, name.c_str());
    }
    glUniform1i(loc, value);
}

std::vector<UniformInfo> Shader::GetCustomUniforms()
{
    std::vector<UniformInfo> uniforms;
    GLint count;
    glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &count);

    for(GLint i = 0; i < count; i++) {
        char name[256];
        GLsizei length;
        GLint size;
        GLenum type;

        glGetActiveUniform(programID, i, sizeof(name), &length, &size, &type, name);
        std::string uniformName(name);
        if(IsStandardUniform(uniformName)) continue;

        GLint location = glGetUniformLocation(programID, name);
        uniforms.push_back({ uniformName, type, location });
    }
    return uniforms;
}
