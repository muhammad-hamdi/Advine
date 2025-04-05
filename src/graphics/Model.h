#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "Mesh.h"

class Model {
public:
    void Draw();
    void LoadGLTF(const std::string& path);

    std::vector<Mesh> GetMeshes() { return meshes; }
    void SetMaterial(Material *mat);

private:
    std::vector<Mesh> meshes;
};

#endif
