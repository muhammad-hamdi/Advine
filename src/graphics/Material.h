#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"
#include "Texture.h" // Assume you have a texture class

class Material {
public:
    Shader* shader;
    Texture* diffuseTexture;
    Texture* specularTexture;

    Material(Shader* shader, Texture* diffuseTexture = nullptr, Texture* specularTexture = nullptr)
        : shader(shader), diffuseTexture(diffuseTexture), specularTexture(specularTexture) {}

    Shader* GetShader() const { return shader; }
    void SetShader(Shader* newShader) {
        shader = newShader;
    }
    void Bind();

};

#endif
