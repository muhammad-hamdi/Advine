#pragma once
#include "render/api/OpenGLRenderAPI.h"

#include "rendering/LightData.h"

#include <vector>
#include <glm/glm.hpp>

class Mesh;
class Material;

namespace Engine
{
    struct CameraData {
        glm::mat4 view;
        glm::mat4 projection;
        glm::vec3 cameraPosition;
        float padding = 0.0f;
    };

    class NRenderer
    {
    private:
        std::shared_ptr<RenderAPI> api;
        CameraData mCurrentCamera;
    public:
        NRenderer();
        NRenderer(RenderBackend backend);
        ~NRenderer();

        std::shared_ptr<RenderAPI> GetAPI();

        void SetCamera(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& pos);
        void ApplyLightUniforms(GPUHandle shader, const std::vector<LightData>& lights);

        void BeginFrame();
        void EndFrame();

        void SubmitMesh(const Mesh &mesh, const Material &material, const glm::mat4 &transform);
    };
} // namespace Engine
