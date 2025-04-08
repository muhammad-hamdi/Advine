#pragma once
#include "core/Component.h"
#include "rendering/Mesh.h"
#include "rendering/Material.h"

class MeshRenderer : public Component {
public:
    std::vector<Mesh*> meshes;
    std::vector<Material*> materials;
};