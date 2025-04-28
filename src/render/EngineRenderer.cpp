#include "EngineRenderer.h"
#include <assert.h>

namespace Engine {
    std::shared_ptr<RenderAPI> CreateRenderAPI(RenderBackend backend) {
        switch (backend) {
            case RenderBackend::OpenGL: return std::make_shared<OpenGLRenderAPI>();
            // case RenderBackend::Vulkan: return std::make_shared<VulkanRenderAPI>();
        }
    }

    std::shared_ptr<RenderAPI> NRRenderer::sAPI;
    void NRRenderer::Init(RenderBackend backend)
    {
        switch (backend)
        {
        case RenderBackend::OpenGL:
            sAPI = std::make_shared<OpenGLRenderAPI>();
            break;
        }
        // sAPI->Init(); // Let API initialize itself (e.g., load GL extensions)
    }

    void NRRenderer::BeginFrame()
    {
        assert(false, "not implemented");
    }

    void NRRenderer::EndFrame()
    {
        assert(false, "not implemented");
    }

    std::shared_ptr<RenderAPI> NRRenderer::GetAPI()
    {
        return sAPI;
    }

    void NRRenderer::SubmitMesh(const Mesh& mesh, const Material& material, const glm::mat4& transform)
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
