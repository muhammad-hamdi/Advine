#include "Material.h"

namespace Engine {
    void Material::Bind()
    {
        if (!shader) return;
        shader->Use();

        int unit = 0;
        for (int i = 0; i < diffuseTextures.size(); i++, unit++) {
            diffuseTextures[i]->Bind(unit);
            // name in shader u_DiffuseTextureN
            shader->SetInt("u_DiffuseTexture" + std::to_string(i + 1), unit);
        }

        for (int i = 0; i < specularTextures.size(); i++, unit++) {
            specularTextures[i]->Bind(unit);
            shader->SetInt("u_SpecularTexture" + std::to_string(i + 1), unit);
        }

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
                // add more types
            }
        }

        shader->ApplyGlobalUniforms();
    }

    void Material::UnBind() {
        if (!shader) return;
        shader->Use();

        int unit = 0;
        for (int i = 0; i < diffuseTextures.size(); i++, unit++) {
            diffuseTextures[i]->UnBind(unit);
            shader->SetInt("u_DiffuseTexture" + std::to_string(i + 1), -1);
        }

        for (int i = 0; i < specularTextures.size(); i++, unit++) {
            specularTextures[i]->UnBind(unit);
            shader->SetInt("u_SpecularTexture" + std::to_string(i + 1), -1);
        }
    }

    const std::vector<UniformInfo>& Material::GetUniformMetadata() const
    {
        return uniformMetadata;
    }
}