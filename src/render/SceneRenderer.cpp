#include "SceneRenderer.h"

#include "core/Game.h"
#include "core/Context.h"

#include "components/Camera.h"
#include "components/MeshRenderer.h"

namespace Engine
{
    SceneRenderer::SceneRenderer(NRenderer *renderer) : renderer(renderer)
    {
    }

    void SceneRenderer::Render(Scene *scene) // maybe replace with a const ref
    {
        // auto camEn = scene->GetActiveCameraEntity();
        // auto cam = camEn->GetComponent<CameraComponent>();
        // renderer->SetCamera(cam->GetViewMatrix(), cam->GetProjectionMatrix((float)Game::windowHeight / Game::windowWidth), camEn->GetWorldPosition());

        lightsToRender.clear();
        scene->GatherLights(lightsToRender);

        for (auto& entity : scene->GetEntities()) {
            RenderEntity(entity);
        }
    }

    void SceneRenderer::RenderEntity(Entity* entity) {
        MeshRenderer* mr = entity->GetComponent<MeshRenderer>();
        if (mr)
            for (int i = 0; i < mr->meshes.size(); i++) {
                if(mr->materials[i]->isLit) {
                    renderer->ApplyLightUniforms(mr->materials[i]->GetShaderHandle(), lightsToRender);
                }
                renderer->SubmitMesh(*mr->meshes[i], *mr->materials[i], entity->GetWorldMatrix());
            }
        for (auto& child : entity->children) {
            RenderEntity(child);
        }
    }
} // namespace Engine
