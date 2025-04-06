#include "Material.h"
#include "Shader.h"
#include "Texture.h"

void Material::Bind()
{
    if (!shader) return;

    shader->Use();

    for (const auto& [name, value] : customUniforms) {
        switch (value.second) {
            case GL_FLOAT:
                shader->SetFloat(name, value.first.f);
                break;
            case GL_INT:
                shader->SetInt(name, value.first.f);
                break;
            case GL_FLOAT_VEC3:
                shader->SetVec3(name, value.first.v3);
                break;
            case GL_FLOAT_MAT4:
                shader->SetMat4(name, value.first.m4);
                break;
            // ... add more types
        }
    }

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

const std::vector<UniformInfo> &Material::GetUniformMetadata() const
{
    return uniformMetadata;
}