#pragma once
#include "render/api/OpenGLRenderAPI.h"

#include <glm/glm.hpp>

class Mesh;
class Material;

namespace Engine
{
    class NRenderer
    {
    private:
        static std::shared_ptr<RenderAPI> sAPI;
    public:
        static void Init(RenderBackend backend);

        static void BeginFrame();
        static void EndFrame();

        static std::shared_ptr<RenderAPI> GetAPI();
        void SubmitMesh(const Mesh &mesh, const Material &material, const glm::mat4 &transform);
    };
} // namespace Engine
