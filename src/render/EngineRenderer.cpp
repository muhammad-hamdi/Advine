#include "EngineRenderer.h"
#include <assert.h>

namespace Engine {
    std::shared_ptr<RenderAPI> CreateRenderAPI(RenderBackend backend) {
        switch (backend) {
            case RenderBackend::OpenGL: return std::make_shared<OpenGLRenderAPI>();
            // case RenderBackend::Vulkan: return std::make_shared<VulkanRenderAPI>();
        }
        assert(false, "failed to create render api");
        return nullptr;
    }

    std::shared_ptr<RenderAPI> NRenderer::sAPI;
    void NRenderer::Init(RenderBackend backend)
    {
        switch (backend)
        {
        case RenderBackend::OpenGL:
            sAPI = std::make_shared<OpenGLRenderAPI>();
            break;
        }
        // sAPI->Init(); // Let API initialize itself (e.g., load GL extensions)
    }

    void NRenderer::BeginFrame()
    {
        assert(false, "not implemented");
    }

    void NRenderer::EndFrame()
    {
        assert(false, "not implemented");
    }

    std::shared_ptr<RenderAPI> NRenderer::GetAPI()
    {
        return sAPI;
    }

    void NRenderer::SubmitMesh(const Mesh& mesh, const Material& material, const glm::mat4& transform)
    {
        // Later: Queue draw calls, sort them, etc.
        // For now: Simple immediate mode submission

        // Set material (shader, uniforms, textures)
        // material.Bind();

        // Bind mesh buffers
        // mesh.Bind();

        // Set transforms (typically UBO or PushConstant update)
        // material.SetTransform(transform);

        // Renderer::GetAPI()->DrawIndexed(mesh.GetIndexCount());
    }
}
