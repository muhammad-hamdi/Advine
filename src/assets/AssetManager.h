#pragma once
#include "assets/Model.h"
#include "rendering/Material.h"

#include "render/NShader.h"
#include "render/NTexture.h"

#include <unordered_set>
#include <unordered_map>
#include <string>
#include <memory>

namespace Engine {
    class AssetManager {
    public:
        static void Init();

        static Model* LoadModel(const std::string& name, const std::string& path);

        static Material* LoadMaterial(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::string& texturePath);

        static void AddMaterial(const std::string& name, Material* mat);

        static Model* GetModel(const std::string& name);
        static Material* GetMaterial(const std::string& name);

        static Material* GetDefaultMaterial();

        static void Clear();

        // new loaders

        static std::string ExtractPathFromInclude(const std::string& include);
        static std::string LoadShaderSource(const std::string& path, std::unordered_set<std::string>& included);
        static NShader* LoadNShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
        static NTexture* LoadNTexture(const std::string& name, const std::string& path);
        static NShader *GetNShader(const std::string &name);
    private:
        static std::unordered_map<std::string, std::unique_ptr<Model>> models;
        static std::unordered_map<std::string, std::unique_ptr<Material>> materials;
        static inline Material* defaultMaterial = nullptr;

        static std::unordered_map<std::string, std::unique_ptr<NShader>> nShaders;
        static std::unordered_map<std::string, std::unique_ptr<NTexture>> nTextures;
    };
}