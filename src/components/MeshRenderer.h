#pragma once
#include "core/Component.h"
#include "rendering/Mesh.h"
#include "rendering/Material.h"

namespace Engine {
    class MeshRenderer : public Component {
    public:
        std::vector<Mesh*> meshes;
        std::vector<std::string> materials;
    };
}