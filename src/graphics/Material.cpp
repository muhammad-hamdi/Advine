#include "Material.h"
#include "Shader.h"
#include "Texture.h"

void Material::Bind()
{
    if (!shader) return;

    shader->Use();

    if (diffuseTexture) {
        diffuseTexture->Bind(0); // Activate and bind to texture unit 0
        shader->SetInt("texture_diffuse", 0); // Assumes uniform name in shader
    }

    if (specularTexture) {
        specularTexture->Bind(1); // Activate and bind to texture unit 1
        shader->SetInt("texture_specular", 1); // Assumes uniform name in shader
    }

    // set more material-related uniforms here if needed
}

void Material::UnBind() {
    if (!shader) return;

    shader->Use();

    if (diffuseTexture) {
        diffuseTexture->UnBind(0);
        shader->SetInt("texture_diffuse", -1);
    }

    if (specularTexture) {
        specularTexture->UnBind(1);
        shader->SetInt("texture_specular", -1);
    }
}