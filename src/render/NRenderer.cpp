#include "NRenderer.h"

#include "rendering/Material.h"
#include "rendering/Mesh.h"

#include <assert.h>

namespace Engine {
    NRenderer::NRenderer()
    {
        api = std::make_unique<OpenGLRenderAPI>();
    }

    NRenderer::NRenderer(RenderBackend backend)
    {
        switch (backend)
        {
            case RenderBackend::OpenGL:
                api = std::make_unique<OpenGLRenderAPI>();
                break;
        }
    }

    NRenderer::~NRenderer()
    {
    }

    std::shared_ptr<RenderAPI> NRenderer::GetAPI()
    {
        return api;
    }

    void NRenderer::SetCamera(const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &pos)
    {
        mCurrentCamera.view = view;
        mCurrentCamera.projection = projection;
        mCurrentCamera.cameraPosition = pos;
    }

    void NRenderer::ApplyLightUniforms(GPUHandle shader, const std::vector<LightData>& lights)
    {
        // TODO: move to UBO
        for (int i = 0; i < lights.size(); ++i) {
            const auto& light = lights[i];
            std::string prefix = "u_Lights[" + std::to_string(i) + "]";

            api->SetUniformInt(shader, prefix + ".type", light.type);
            api->SetUniformVec3(shader, prefix + ".color", glm::value_ptr(light.color));
            api->SetUniformVec3(shader, prefix + ".position", glm::value_ptr(light.position));
            api->SetUniformFloat(shader, prefix + ".range", light.range);
            api->SetUniformFloat(shader, prefix + ".constant", light.constant);
            api->SetUniformFloat(shader, prefix + ".linear", light.linear);
            api->SetUniformFloat(shader, prefix + ".quadratic", light.quadratic);
            api->SetUniformVec3(shader, prefix + ".direction", glm::value_ptr(light.direction));
            api->SetUniformFloat(shader, prefix + ".spotAngle", light.spotAngle);
            api->SetUniformInt(shader, prefix + ".castShadows", light.castShadows);
        }

        api->SetUniformInt(shader, "u_LightCount", static_cast<int>(lights.size()));
        api->SetUniformInt(shader, "u_IsLit", true);
    }

    void NRenderer::BeginFrame()
    {
        assert(false, "not implemented");
    }

    void NRenderer::EndFrame()
    {
        assert(false, "not implemented");
    }

    void NRenderer::SubmitMesh(const Mesh& mesh, const Material& material, const glm::mat4& transform)
    {
        GPUHandle shader = material.GetShaderHandle();
        //  // 1. Bind the shader
        api->BindShader(shader);

        // // 2. Bind the textures
        int slot = 0;
        for (auto& [name, texture] : material.GetTextures()) {
            api->BindTexture(texture->handle, slot);
            api->SetUniformInt(shader, name, slot++);
        }

        // TODO: move transformation matrices to UBO
        api->SetUniformMat4(shader, "u_Model", glm::value_ptr(transform));
        api->SetUniformMat4(shader, "u_View", glm::value_ptr(mCurrentCamera.view));
        api->SetUniformMat4(shader, "u_Projection", glm::value_ptr(mCurrentCamera.projection));
        api->SetUniformMat4(shader, "u_CameraPos", glm::value_ptr(mCurrentCamera.cameraPosition));

        // // 3. Upload transforms or material data
        // api->UpdateUniformBuffer(material.GetTransformUBO(), &transform, sizeof(glm::mat4));

        // // 4. Bind vertex/index buffers
        api->BindVertexBuffer(mesh.GetVertexBufferHandle());
        api->BindIndexBuffer(mesh.GetIndexBufferHandle());
        api->BindVertexDescription(mesh.GetVertexDescriptionHandle());

        // // 5. Submit draw call
        api->DrawIndexed(mesh.GetIndexCount());
    }
}
