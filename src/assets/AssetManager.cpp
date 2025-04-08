#include "AssetManager.h"

std::unordered_map<std::string, std::unique_ptr<Model>> AssetManager::models;
std::unordered_map<std::string, std::unique_ptr<Shader>> AssetManager::shaders;
std::unordered_map<std::string, std::unique_ptr<Texture>> AssetManager::textures;
std::unordered_map<std::string, std::unique_ptr<Material>> AssetManager::materials;

void AssetManager::Init() {
    Shader* defaultShader = LoadShader("default", "assets/shaders/default.vert", "assets/shaders/default.frag");
    defaultMaterial = new Material("default", defaultShader);
}

Model* AssetManager::LoadModel(const std::string& name, const std::string& path) {
    auto it = models.find(name);
    if (it != models.end()) return it->second.get();

    auto model = std::make_unique<Model>();
    model->LoadGLTF(path);
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
    auto newMaterial = std::make_unique<Material>(name, shader, texture);

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

Texture* AssetManager::GetTexture(const std::string& name) {
    auto it = textures.find(name);
    return (it != textures.end()) ? it->second.get() : nullptr;
}

Material* AssetManager::GetMaterial(const std::string& name) {
    auto it = materials.find(name);
    return (it != materials.end()) ? it->second.get() : nullptr;
}

Material *AssetManager::GetDefaultMaterial()
{
    return defaultMaterial;
}

void AssetManager::Clear() {
    models.clear();
    shaders.clear();
    textures.clear();
}
