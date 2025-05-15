#include "SceneRenderer.h"

#include "core/Game.h"
#include "core/Context.h"

#include "assets/AssetManager.h"

#include "components/Camera.h"
#include "components/MeshRenderer.h"

namespace Engine
{
    SceneRenderer::SceneRenderer(NRenderer *renderer) : renderer(renderer)
    {}

    void SceneRenderer::Init() {
        depthFramebuffer = renderer->GetAPI()->CreateFramebuffer();
        depthMap = renderer->GetAPI()->CreateTextureDepth(SHADOW_WIDTH, SHADOW_HEIGHT);
        renderer->GetAPI()->AttachDepthBuffer(depthFramebuffer, depthMap);
    }

    void SceneRenderer::Render(Scene *scene) // maybe replace with a const ref
    {
        auto api = renderer->GetAPI();
        auto window = Context::Get().GetWindowDims();

#pragma region lightDepthBuffer render pass
        api->SetViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        api->BindFramebuffer(depthFramebuffer);
        api->ClearDepth();

        auto lightEnt = scene->GetDirectionalLightEntity();
        float near_plane = 1.0f, far_plane = 100.0f, orthoSize = 30.0f;
        glm::mat4 lightProjection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, near_plane, far_plane);
        glm::mat4 lightView = glm::lookAt(-lightEnt->transform.GetForwardDirection() * 20.0f, glm::vec3( 0.0f, 0.0f,  0.0f), glm::vec3( 0.0f, 1.0f,  0.0f));
        lightSpaceMatrix = lightProjection * lightView;
        auto shader = AssetManager::GetNShader("simple_depth_shader");

        api->BindShader(shader->handle);
        api->SetUniformMat4(shader->handle, "u_LightSpaceMatrix", glm::value_ptr(lightSpaceMatrix));

        for (auto& entity : scene->GetEntities()) {
            RenderEntityWithShader(entity, shader->handle);
        }
        // render scene
        api->BindFramebuffer(0); // very opengl behavior, will not work when handles become indices in gpu object arrays in other APIs, todo: change
#pragma endregion

#pragma region standard render pass
        renderer->BeginFrame();

        api->SetViewport(0, 0, window.first, window.second);
        api->BindTexture(depthMap, 0);
        lightsToRender.clear();
        scene->GatherLights(lightsToRender);

        for (auto& entity : scene->GetEntities()) {
            RenderEntity(entity);
        }
        renderer->EndFrame();
#pragma endregion
    }

    void SceneRenderer::RenderEntity(Entity* entity) {
        MeshRenderer* mr = entity->GetComponent<MeshRenderer>();
        if (mr)
            for (int i = 0; i < mr->meshes.size(); i++) {
                if(mr->materials[i]->isLit) {
                    renderer->ApplyLightUniforms(mr->materials[i]->GetShaderHandle(), lightsToRender, lightSpaceMatrix);
                }
                renderer->SubmitMesh(*mr->meshes[i], *mr->materials[i], entity->GetWorldMatrix());
            }
        for (auto& child : entity->children) {
            RenderEntity(child);
        }
    }

    void SceneRenderer::RenderEntityWithShader(Entity* entity, GPUHandle shader) {
        MeshRenderer* mr = entity->GetComponent<MeshRenderer>();
        auto api = renderer->GetAPI();
        if (mr) {
            for (int i = 0; i < mr->meshes.size(); i++) {
                auto mesh = *mr->meshes[i];
                api->SetUniformMat4(shader, "u_Model", glm::value_ptr(entity->GetWorldMatrix()));

                api->BindVertexDescription(mesh.GetVertexDescriptionHandle());
                api->BindVertexBuffer(mesh.GetVertexBufferHandle());
                api->BindIndexBuffer(mesh.GetIndexBufferHandle());

                // Submit draw call
                api->DrawIndexed(mesh.GetIndexCount());
            }
        }
        for (auto& child : entity->children) {
            RenderEntityWithShader(child, shader);
        }
    }
} // namespace Engine
