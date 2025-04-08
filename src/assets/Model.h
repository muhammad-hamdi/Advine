#ifndef MODEL_H
#define MODEL_H

#include "rendering/Mesh.h"
#include "core/Entity.h"

#include <assimp/Importer.hpp>      // Importer for Assimp
#include <assimp/scene.h>           // Structure that contains the model data
#include <assimp/postprocess.h>     // Post-process steps (for example, scaling, normalizing, etc.)

#include <vector>
#include <filesystem>

class Model {
public:
    void Draw();
    void LoadGLTF(const std::string& path);

    std::vector<Mesh> GetMeshes() { return meshes; }
    void SetMaterial(Material *mat);

    Entity *LoadAssimp(const std::string &path);

    Entity *ProcessNode(aiNode *node, const aiScene *scene, const std::filesystem::path &baseDir);

    Mesh *ProcessMesh(aiMesh *mesh);

    Material *LoadMaterial(aiMaterial *aiMat, const std::filesystem::path &baseDir);

private:
    std::vector<Mesh> meshes;
};

#endif
