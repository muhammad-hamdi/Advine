#include "Model.h"

#include "rendering/Mesh.h"
#include "rendering/Material.h"
#include "assets/AssetManager.h"
#include "components/MeshRenderer.h"

#include <iostream>
#include <algorithm>

namespace Engine {
    void Model::SetMaterial(Material* mat) {

    }

    Entity* Model::LoadAssimp(const std::string& path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path,
            aiProcess_Triangulate |
            aiProcess_GenNormals |
            aiProcess_JoinIdenticalVertices |
            aiProcess_FindInstances |
            aiProcess_OptimizeMeshes |
            aiProcess_FindInvalidData);

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

        if (node->mNumMeshes > 0) {
            MeshRenderer* renderer = entity->AddComponent<MeshRenderer>();
            for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                renderer->meshes.push_back(ProcessMesh(mesh));
                renderer->meshes.back()->SetMaterial(LoadMaterial(scene->mMaterials[mesh->mMaterialIndex], baseDir));
            }
            std::sort(renderer->meshes.begin(), renderer->meshes.end(), [](Mesh* a, Mesh* b) {
                return a->GetMaterial() < b->GetMaterial();
            });
        }

        for (unsigned int i = 0; i < node->mNumChildren; ++i) {
            Entity* child = ProcessNode(node->mChildren[i], scene, baseDir);
            child->parent = entity;
            entity->children.push_back(child);
        }

        return entity;
    }

    Mesh* Model::ProcessMesh(aiMesh* mesh) {
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

    std::string Model::LoadMaterial(aiMaterial* aiMat, const std::filesystem::path& baseDir) {
        aiString name;
        aiMat->Get(AI_MATKEY_NAME, name);

        std::string matName = name.C_Str();
        if (auto* cached = AssetManager::GetMaterial(matName))
            return matName;

        NShader* shader = AssetManager::GetNShader("default");
        if (!shader)
            shader = AssetManager::LoadNShader("default", "assets/shaders/default.vert", "assets/shaders/default.frag");

        Material* material = new Material(matName, shader);

        // PBR texture loading
        aiString texPath;
        if (aiMat->GetTexture(aiTextureType_BASE_COLOR, 0, &texPath) == AI_SUCCESS) {
            std::string fullPath = (baseDir / texPath.C_Str()).string();
            material->AddTexture("u_AlbedoMap", AssetManager::LoadNTexture(texPath.C_Str(), fullPath));
        } else if (aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS) {
            std::string fullPath = (baseDir / texPath.C_Str()).string();
            material->AddTexture("u_AlbedoMap", AssetManager::LoadNTexture(texPath.C_Str(), fullPath));
        }

        if (aiMat->GetTexture(aiTextureType_NORMALS, 0, &texPath) == AI_SUCCESS) {
            std::string fullPath = (baseDir / texPath.C_Str()).string();
            material->AddTexture("u_NormalMap", AssetManager::LoadNTexture(texPath.C_Str(), fullPath));
        }

        if (aiMat->GetTexture(aiTextureType_METALNESS, 0, &texPath) == AI_SUCCESS) {
            std::string fullPath = (baseDir / texPath.C_Str()).string();
            material->AddTexture("u_MetallicMap", AssetManager::LoadNTexture(texPath.C_Str(), fullPath));
        }

        if (aiMat->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &texPath) == AI_SUCCESS) {
            std::string fullPath = (baseDir / texPath.C_Str()).string();
            material->AddTexture("u_RoughnessMap", AssetManager::LoadNTexture(texPath.C_Str(), fullPath));
        }

        if (aiMat->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &texPath) == AI_SUCCESS) {
            std::string fullPath = (baseDir / texPath.C_Str()).string();
            material->AddTexture("u_AoMap", AssetManager::LoadNTexture(texPath.C_Str(), fullPath));
        }

        aiColor3D color(0.f, 0.f, 0.f);
        if (aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
            material->albedo = glm::vec3(color.r, color.g, color.b);
        }

        float metallic = 0.01f;
        if (aiMat->Get(AI_MATKEY_METALLIC_FACTOR, metallic) == AI_SUCCESS) {
            material->metallic = metallic;
        }

        float roughness = 0.8f;
        if (aiMat->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness) == AI_SUCCESS) {
            material->roughness = roughness;
        }

        material->ao = 0.5f;

        AssetManager::AddMaterial(matName, material);
        return matName;
    }
}