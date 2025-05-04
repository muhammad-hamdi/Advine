#include "AssetManager.h"

#include "core/Context.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <filesystem>
#include <stb_image.h>

namespace Engine {

    std::unordered_map<std::string, std::unique_ptr<Model>> AssetManager::models;
    std::unordered_map<std::string, std::unique_ptr<Shader>> AssetManager::shaders;
    std::unordered_map<std::string, std::unique_ptr<Texture>> AssetManager::textures;
    std::unordered_map<std::string, std::unique_ptr<Material>> AssetManager::materials;
    std::unordered_map<std::string, std::unique_ptr<NShader>> AssetManager::nShaders;
    std::unordered_map<std::string, std::unique_ptr<NTexture>> AssetManager::nTextures;

    void AssetManager::Init() {
        Shader* defaultShader = LoadShader("default", "assets/shaders/default.vert", "assets/shaders/default.frag");
        defaultMaterial = new Material("default", defaultShader);
    }

    Model* AssetManager::LoadModel(const std::string& name, const std::string& path) {
        // TODO: rewrite to use new load or retire completely
        auto it = models.find(name);
        if (it != models.end()) return it->second.get();

        auto model = std::make_unique<Model>();
        // model->LoadGLTF(path);
        models[name] = std::move(model);
        return models[name].get();
    }

    Shader* AssetManager::LoadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath) {
        auto it = shaders.find(name);
        if (it != shaders.end()) {
            printf("INFO: Loaded Default Shader");
            return it->second.get();
        }
        auto shader = std::make_unique<Shader>(vertexPath.c_str(), fragmentPath.c_str());
        shaders[name] = std::move(shader);
        return shaders[name].get();
    }

    Texture* AssetManager::LoadTexture(const std::string& name, const std::string& path) {
        auto it = textures.find(name);
        if (it != textures.end()) return it->second.get();

        auto texture = std::make_unique<Texture>(path);
        textures[name] = std::move(texture);
        return textures[name].get();
    }

    Material* AssetManager::LoadMaterial(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::string& texturePath) {
        if (materials.find(name) != materials.end()) {
            return materials[name].get();
        }

        Shader* shader = LoadShader(vertexPath + fragmentPath, vertexPath, fragmentPath);
        Texture* texture = LoadTexture(texturePath, texturePath);
        auto newMaterial = std::make_unique<Material>(name, shader);
        newMaterial.get()->diffuseTextures.push_back(texture);

        materials[name] = std::move(newMaterial);
        return materials[name].get();
    }

    void AssetManager::AddMaterial(const std::string& name, Material* mat) {
        auto it = materials.find(name);
        if (it != materials.end()) return; //different models might have same material names, need to find a better way to handle this

        materials[name] = std::move(std::unique_ptr<Material>(mat));
    }

    Model* AssetManager::GetModel(const std::string& name) {
        auto it = models.find(name);
        return (it != models.end()) ? it->second.get() : nullptr;
    }

    Shader* AssetManager::GetShader(const std::string& name) {
        printf("INFO: Loading Shader {%s}\n", name.c_str());
        auto it = shaders.find(name);
        return (it != shaders.end()) ? it->second.get() : nullptr;
    }

    NShader* AssetManager::GetNShader(const std::string& name) {
        printf("INFO: Loading Shader {%s}\n", name.c_str());
        auto it = nShaders.find(name);
        return (it != nShaders.end()) ? it->second.get() : nullptr;
    }

    Texture* AssetManager::GetTexture(const std::string& name) {
        auto it = textures.find(name);
        return (it != textures.end()) ? it->second.get() : nullptr;
    }

    Material* AssetManager::GetMaterial(const std::string& name) {
        auto it = materials.find(name);
        return (it != materials.end()) ? it->second.get() : nullptr;
    }

    Material* AssetManager::GetDefaultMaterial()
    {
        return defaultMaterial;
    }

    void AssetManager::Clear() {
        models.clear();
        shaders.clear();
        textures.clear();
    }

    std::string AssetManager::ExtractPathFromInclude(const std::string& include)
    {
        int s = 0, e = 0;
        for (int i = 0; i < include.size(); i++) {
            if (s == 0) {
                if (include[i] == '"' || include[i] == '<') {
                    s = i + 1;
                }
            }
            else {
                if (include[i] == '"' || include[i] == '>') {
                    e = i;
                }
            }
        }
        std::string path = include.substr(s, e - s);
        return path;
    }

    std::string AssetManager::LoadShaderSource(const std::string &path, std::unordered_set<std::string> &included)
    {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Failed to open shader file: " << path << std::endl;
            return 0;
        }

        std::stringstream output;
        std::string line;
        std::string dir = std::filesystem::path(path).parent_path().string();

        while (std::getline(file, line)) {
            if (line.rfind("#include", 0) == 0) {
                std::string includePath = ExtractPathFromInclude(line); // e.g. from #include "common.glsl"
                std::string fullPath = dir + "/" + includePath;

                // Avoid recursive includes
                if (included.count(fullPath)) continue;
                included.insert(fullPath);

                output << LoadShaderSource(fullPath, included) << "\n";
            }
            else {
                output << line << "\n";
            }
        }

        return output.str();
    }
    NShader *AssetManager::LoadNShader(const std::string &name, const std::string &vertexPath, const std::string &fragmentPath)
    {
        auto it = nShaders.find(name);
        if (it != nShaders.end()) {
            // printf("INFO: Loaded Default Shader");
            return it->second.get();
        }
        auto api = Context::Get().GetRenderer()->GetAPI();
        auto shader = std::make_unique<NShader>();
        std::unordered_set<std::string> included;
        std::string vertexSrc = LoadShaderSource(vertexPath, included);
        included.clear();
        std::string fragmentSrc = LoadShaderSource(fragmentPath, included);
        shader->handle = api->CreateShader(vertexSrc, fragmentSrc);
        shader->vertexPath = vertexPath;
        shader->fragmentPath = fragmentPath;
        nShaders[name] = std::move(shader);
        return nShaders[name].get();
    }

    NTexture *AssetManager::LoadNTexture(const std::string &name, const std::string &path)
    {
        auto it = nTextures.find(name);
        if (it != nTextures.end()) return it->second.get();

        auto api = Context::Get().GetRenderer()->GetAPI();
        auto texture = std::make_unique<NTexture>();

        stbi_set_flip_vertically_on_load(true); // Flip to match OpenGL convention
        unsigned char* data = stbi_load(path.c_str(), &texture->width, &texture->height, &texture->channels, 0);
        if (data) {
            texture->handle = api->CreateTexture2D(texture->width, texture->height, data, texture->channels);
            texture->filepath = path;
        }
        else {
            std::cerr << "Failed to load texture: " << path << std::endl;
        }
        stbi_image_free(data);

        nTextures[name] = std::move(texture);
        return nTextures[name].get();
    }
}