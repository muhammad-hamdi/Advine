#pragma once
#include "render/api/OpenGLRenderAPI.h"

#include "rendering/LightData.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

namespace Engine
{
    struct CameraData {
        glm::mat4 view;
        glm::mat4 projection;
        glm::vec3 cameraPosition;
        float padding = 0.0f;
    };

    class Mesh;
    class Material;

    class NRenderer
    {
    private:
        std::shared_ptr<RenderAPI> api;
        CameraData mCurrentCamera;
        Material* boundMaterial;
    public:
        NRenderer();
        NRenderer(RenderBackend backend);
        ~NRenderer();

        std::shared_ptr<RenderAPI> GetAPI();

        inline CameraData GetCamera() const { return mCurrentCamera; };
        void SetCamera(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& pos);
        void ApplyLightUniforms(GPUHandle shader, const std::vector<LightData> &lights, const glm::mat4 &lightSpaceMatrix);
        void ApplyLightUniforms(GPUHandle shader, const std::vector<LightData> &lights);

        void BeginFrame();
        void EndFrame();

        void SubmitMesh(const Mesh &mesh, Material &material, const glm::mat4 &transform);
    };
} // namespace Engine
