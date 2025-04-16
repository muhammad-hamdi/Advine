#include "Model.h"

#include "rendering/Mesh.h"
#include "rendering/Material.h"
#include "assets/Texture.h"
#include "assets/AssetManager.h"
#include "components/MeshRenderer.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>
#include <iostream>

void Model::LoadGLTF(const std::string& path) {
    tinygltf::TinyGLTF loader;
    tinygltf::Model gltfModel;
    std::string err, warn;

    bool ret = loader.LoadASCIIFromFile(&gltfModel, &err, &warn, path);

    if (!warn.empty()) std::cout << "GLTF warning: " << warn << std::endl;
    if (!err.empty()) std::cerr << "GLTF error: " << err << std::endl;
    if (!ret) {
        std::cerr << "Failed to load glTF: " << path << std::endl;
        return;
    }

    for (const auto& mesh : gltfModel.meshes) {
        for (const auto& primitive : mesh.primitives) {
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;

            const float* posBuffer = nullptr;
            const float* normBuffer = nullptr;
            const float* texCoordBuffer = nullptr;

            // POSITION
            if (primitive.attributes.find("POSITION") != primitive.attributes.end()) {
                const auto& accessor = gltfModel.accessors[primitive.attributes.at("POSITION")];
                const auto& bufferView = gltfModel.bufferViews[accessor.bufferView];
                posBuffer = reinterpret_cast<const float*>(
                    &gltfModel.buffers[bufferView.buffer].data[bufferView.byteOffset + accessor.byteOffset]);
            }

            // NORMAL
            if (primitive.attributes.find("NORMAL") != primitive.attributes.end()) {
                const auto& accessor = gltfModel.accessors[primitive.attributes.at("NORMAL")];
                const auto& bufferView = gltfModel.bufferViews[accessor.bufferView];
                normBuffer = reinterpret_cast<const float*>(
                    &gltfModel.buffers[bufferView.buffer].data[bufferView.byteOffset + accessor.byteOffset]);
            }

            // TEXCOORD_0
            if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end()) {
                const auto& accessor = gltfModel.accessors[primitive.attributes.at("TEXCOORD_0")];
                const auto& bufferView = gltfModel.bufferViews[accessor.bufferView];
                texCoordBuffer = reinterpret_cast<const float*>(
                    &gltfModel.buffers[bufferView.buffer].data[bufferView.byteOffset + accessor.byteOffset]);
            }

            // Vertices
            const auto& posAccessor = gltfModel.accessors[primitive.attributes.at("POSITION")];
            for (size_t i = 0; i < posAccessor.count; i++) {
                Vertex vertex;

                vertex.position = glm::vec3(
                    posBuffer[i * 3 + 0],
                    posBuffer[i * 3 + 1],
                    posBuffer[i * 3 + 2]
                );

                if (normBuffer) {
                    vertex.normal = glm::vec3(
                        normBuffer[i * 3 + 0],
                        normBuffer[i * 3 + 1],
                        normBuffer[i * 3 + 2]
                    );
                } else {
                    vertex.normal = glm::vec3(0.0f);
                }

                if (texCoordBuffer) {
                    vertex.texCoords = glm::vec2(
                        texCoordBuffer[i * 2 + 0],
                        texCoordBuffer[i * 2 + 1]
                    );
                } else {
                    vertex.texCoords = glm::vec2(0.0f);
                }

                vertices.push_back(vertex);
            }

            // Indices
            const auto& indexAccessor = gltfModel.accessors[primitive.indices];
            const auto& bufferView = gltfModel.bufferViews[indexAccessor.bufferView];
            const auto& buffer = gltfModel.buffers[bufferView.buffer];

            const void* dataPtr = &buffer.data[bufferView.byteOffset + indexAccessor.byteOffset];

            for (size_t i = 0; i < indexAccessor.count; i++) {
                unsigned int index = 0;
                switch (indexAccessor.componentType) {
                    case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE:
                        index = ((uint8_t*)dataPtr)[i]; break;
                    case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT:
                        index = ((uint16_t*)dataPtr)[i]; break;
                    case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT:
                        index = ((uint32_t*)dataPtr)[i]; break;
                }
                indices.push_back(index);
            }

            int materialIndex = primitive.material;
            Material* mat = nullptr;

            if (materialIndex >= 0 && materialIndex < gltfModel.materials.size()) {
                const tinygltf::Material& gltfMaterial = gltfModel.materials[materialIndex];

                Texture* diffuseTexture = nullptr;

                // Load diffuse texture if available
                if (gltfMaterial.values.find("baseColorTexture") != gltfMaterial.values.end()) {
                    const auto& texInfo = gltfMaterial.values.at("baseColorTexture");
                    int textureIndex = texInfo.TextureIndex();
                    if (textureIndex >= 0 && textureIndex < gltfModel.textures.size()) {
                        const tinygltf::Texture& gltfTexture = gltfModel.textures[textureIndex];
                        int imageIndex = gltfTexture.source;

                        if (imageIndex >= 0 && imageIndex < gltfModel.images.size()) {
                            const tinygltf::Image& image = gltfModel.images[imageIndex];
                            std::string texName = image.uri;

                            // Load texture via AssetManager
                            diffuseTexture = AssetManager::LoadTexture(texName, "assets/models/" + texName);
                        }
                    }
                }

                // Load shader from AssetManager or default
                Shader* shader = AssetManager::GetShader("default");
                if (!shader) {
                    printf("WARNING: Failed To Load Default Shader\n");
                    shader = AssetManager::LoadShader("default", "assets/shaders/default.vert", "assets/shaders/default.frag");
                }

                mat = new Material(gltfMaterial.name, shader, diffuseTexture);
                AssetManager::AddMaterial(gltfMaterial.name, mat);
                printf("INFO: Custom Material Loaded\n");
            } else {
                // Fallback to default material
                mat = AssetManager::GetDefaultMaterial();
                printf("INFO: Default Material Loaded\n");
            }
            meshes.push_back(Mesh(vertices, indices, mat));
        }
    }
}

void Model::SetMaterial(Material* mat) {
    for (Mesh& mesh : meshes) {
        mesh.SetMaterial(mat);
    }
}

Entity *Model::LoadAssimp(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_GenNormals |
        aiProcess_JoinIdenticalVertices);

    if (!scene || !scene->mRootNode) {
        std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    std::filesystem::path baseDir = std::filesystem::path(path).parent_path();
    Entity* rootEntity = ProcessNode(scene->mRootNode, scene, baseDir);
    rootEntity->isModelRoot = true;
    return rootEntity;
}

Transform ConvertTransform(const aiMatrix4x4& mat) {
    aiVector3D scaling, position;
    aiQuaternion rotation;
    mat.Decompose(scaling, rotation, position);

    Transform t;
    t.position = glm::vec3(position.x, position.y, position.z);
    t.rotation = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);
    t.scale = glm::vec3(scaling.x, scaling.y, scaling.z);
    return t;
}

Entity* Model::ProcessNode(aiNode* node, const aiScene* scene, const std::filesystem::path& baseDir) {
    Entity* entity = new Entity();
    entity->name = node->mName.C_Str();
    entity->transform = ConvertTransform(node->mTransformation);

    if(node->mNumMeshes > 0) {
        MeshRenderer* renderer = entity->AddComponent<MeshRenderer>();
        for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    
            renderer->meshes.push_back(ProcessMesh(mesh));
            renderer->materials.push_back(LoadMaterial(scene->mMaterials[mesh->mMaterialIndex], baseDir));
        }
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        Entity* child = ProcessNode(node->mChildren[i], scene, baseDir);
        child->parent = entity;
        entity->children.push_back(child);
    }

    return entity;
}

Mesh *Model::ProcessMesh(aiMesh* mesh) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;
        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

        if (mesh->HasNormals())
            vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

        if (mesh->HasTextureCoords(0))
            vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }

    return new Mesh(vertices, indices);
}

Material* Model::LoadMaterial(aiMaterial* aiMat, const std::filesystem::path& baseDir) {
    aiString name;
    aiMat->Get(AI_MATKEY_NAME, name);

    std::string matName = name.C_Str();
    if (auto* cached = AssetManager::GetMaterial(matName))
        return cached;
    
    Shader* shader = AssetManager::GetShader("default");
    if (!shader)
        shader = AssetManager::LoadShader("default", "assets/shaders/default.vert", "assets/shaders/default.frag");    
    
    Material* material = new Material(matName, shader);

    for(int i = 0; i < aiMat->GetTextureCount(aiTextureType_DIFFUSE); i++) {
        aiString texPath;
        aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);
        std::string fullPath = (baseDir / texPath.C_Str()).string();
        material->diffuseTextures.push_back(AssetManager::LoadTexture(texPath.C_Str(), fullPath));
    }

    for(int i = 0; i < aiMat->GetTextureCount(aiTextureType_SPECULAR); i++) {
        aiString texPath;
        aiMat->GetTexture(aiTextureType_SPECULAR, 0, &texPath);
        std::string fullPath = (baseDir / texPath.C_Str()).string();
        material->specularTextures.push_back(AssetManager::LoadTexture(texPath.C_Str(), fullPath));
    }

    AssetManager::AddMaterial(matName, material);
    return material;
}
