#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include "assets/Model.h"
#include "assets/Shader.h"
#include "assets/Texture.h"
#include "rendering/Material.h"

namespace Engine {
    class AssetManager {
    public:
        static void Init();

        static Model* LoadModel(const std::string& name, const std::string& path);
        static Shader* LoadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
        static Texture* LoadTexture(const std::string& name, const std::string& path);

        static Material* LoadMaterial(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::string& texturePath);

        static void AddMaterial(const std::string& name, Material* mat);

        static Model* GetModel(const std::string& name);
        static Shader* GetShader(const std::string& name);
        static Texture* GetTexture(const std::string& name);
        static Material* GetMaterial(const std::string& name);

        static Material* GetDefaultMaterial();

        static void Clear();

    private:
        static std::unordered_map<std::string, std::unique_ptr<Model>> models;
        static std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;
        static std::unordered_map<std::string, std::unique_ptr<Texture>> textures;
        static std::unordered_map<std::string, std::unique_ptr<Material>> materials;
        static inline Material* defaultMaterial = nullptr;
    };
}