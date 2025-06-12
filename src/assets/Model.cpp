#include "Model.h"

#include "rendering/Mesh.h"
#include "rendering/Material.h"
#include "assets/AssetManager.h"
#include "components/MeshRenderer.h"

#include <iostream>

namespace Engine {
    void Model::SetMaterial(Material* mat) {

    }

    Entity* Model::LoadAssimp(const std::string& path) {
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

        // if (node->mNumMeshes > 0) {
            MeshRenderer* renderer = entity->AddComponent<MeshRenderer>();
            ProcessChildren(node, scene, baseDir, renderer);
        // }

        // for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        //     Entity* child = ProcessNode(node->mChildren[i], scene, baseDir);
        //     child->parent = entity;
        //     entity->children.push_back(child);
        // }

        return entity;
    }

    void Model::ProcessChildren(aiNode* parent, const aiScene* scene, const std::filesystem::path& baseDir, MeshRenderer* renderer) {
        for (unsigned int i = 0; i < parent->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[parent->mMeshes[i]];

            renderer->meshes.push_back(ProcessMesh(mesh));
            renderer->materials.push_back(LoadMaterial(scene->mMaterials[mesh->mMaterialIndex], baseDir));
        }
        for (unsigned int i = 0; i < parent->mNumChildren; ++i) {
            ProcessChildren(parent->mChildren[i], scene, baseDir, renderer);
        }
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

    Material* Model::LoadMaterial(aiMaterial* aiMat, const std::filesystem::path& baseDir) {
        aiString name;
        aiMat->Get(AI_MATKEY_NAME, name);

        std::string matName = name.C_Str();
        if (auto* cached = AssetManager::GetMaterial(matName))
            return cached;

        NShader* shader = AssetManager::GetNShader("default");
        if (!shader)
            shader = AssetManager::LoadNShader("default", "assets/shaders/default.vert", "assets/shaders/default.frag");

        Material* material = new Material(matName, shader);

        for (size_t i = 0; i < aiMat->GetTextureCount(aiTextureType_DIFFUSE); i++) {
            aiString texPath;
            aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);
            std::string fullPath = (baseDir / texPath.C_Str()).string();
            material->AddTexture("u_DiffuseTexture" + std::to_string(i + 1), AssetManager::LoadNTexture(texPath.C_Str(), fullPath));
        }

        for (size_t i = 0; i < aiMat->GetTextureCount(aiTextureType_SPECULAR); i++) {
            aiString texPath;
            aiMat->GetTexture(aiTextureType_SPECULAR, 0, &texPath);
            std::string fullPath = (baseDir / texPath.C_Str()).string();
            material->AddTexture("u_SpecularTexture" + std::to_string(i + 1), AssetManager::LoadNTexture(texPath.C_Str(), fullPath));
        }

        AssetManager::AddMaterial(matName, material);
        return material;
    }
}