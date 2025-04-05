#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"
#include "Texture.h"

class Material {
public:
    std::string name;
    Shader* shader;
    Texture* diffuseTexture;
    Texture* specularTexture;

    Material(const std::string& name, Shader* shader, Texture* diffuseTexture = nullptr, Texture* specularTexture = nullptr)
        : name(name), shader(shader), diffuseTexture(diffuseTexture), specularTexture(specularTexture) {}

    Shader* GetShader() const { return shader; }
    void SetShader(Shader* newShader) {
        shader = newShader;
    }
    void Bind();
    void UnBind();
};

#endif
