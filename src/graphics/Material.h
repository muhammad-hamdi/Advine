#ifndef MATERIAL_H
#define MATERIAL_H

#include <variant>
#include <vector>
#include <unordered_map>
#include "Shader.h"
#include "Texture.h"

union UniformValue {
    float f;
    int i;
    glm::vec3 v3;
    glm::mat4 m4;

    UniformValue() : f(0.0f) {}
    UniformValue(float f) : f(f) {}
    UniformValue(int i) : i(i) {}
    UniformValue(glm::vec3 v3) : v3(v3) {}
    UniformValue(glm::mat4 m4) : m4(m4) {}
};

class Material {
public:
    std::string name;
    Shader* shader;
    Texture* diffuseTexture;
    Texture* specularTexture;

    Material(const std::string& name, Shader* shader, Texture* diffuseTexture = nullptr, Texture* specularTexture = nullptr)
        : name(name), shader(shader), diffuseTexture(diffuseTexture), specularTexture(specularTexture) {
            if (shader) {
                uniformMetadata = shader->GetCustomUniforms();
                GetUniformValues();
            }
        }

    Shader* GetShader() const { return shader; }
    void SetShader(Shader* newShader) {
        shader = newShader;
        if (shader) {
            uniformMetadata = shader->GetCustomUniforms();  // <- store info
            GetUniformValues();
        }
    }
    void Bind();
    void UnBind();

    void SetCustomUniform(const std::string& name, const UniformValue& value, GLenum type) {
        customUniforms[name] = {value, type};
    }

    void GetUniformValues() {
        for(auto info: uniformMetadata) {
            UniformValue val;
            switch (info.type) {
                case GL_FLOAT:
                    glGetUniformfv(shader->programID, info.location, &val.f);
                    break;
                case GL_INT:
                    glGetUniformiv(shader->programID, info.location, &val.i);
                    break;
                case GL_FLOAT_VEC3:
                    glGetUniformfv(shader->programID, info.location, glm::value_ptr(val.v3));
                    break;
                case GL_FLOAT_MAT4:
                    glGetUniformfv(shader->programID, info.location, glm::value_ptr(val.m4));
                    break;
                // ... add more types
            }
            customUniforms[info.name] = {val, info.type};
        }
    }

    const std::vector<UniformInfo>& GetUniformMetadata() const;

    std::vector<UniformInfo> uniformMetadata;
    std::unordered_map<std::string, std::pair<UniformValue, GLenum>> customUniforms;
};

#endif
