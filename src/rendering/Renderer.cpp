#include "Renderer.h"
#include "core/Scene.h"
#include "core/Entity.h"
#include "components/MeshRenderer.h"

namespace Engine {
    void Renderer::RenderScene(Scene& scene)
    {
        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        lightsToRender.clear();
        scene.GatherLights(lightsToRender);

        for (auto& entity : scene.GetEntities()) {
            RenderEntity(entity);
        }
    }

    void Renderer::RenderEntity(Entity* entity) {
        MeshRenderer* mr = entity->GetComponent<MeshRenderer>();
        if (mr)
            for (int i = 0; i < mr->meshes.size(); i++) {
                DrawMesh(mr->meshes[i], mr->materials[i], entity->GetWorldMatrix());
            }
        for (auto& child : entity->children) {
            RenderEntity(child);
        }
    }


    void Renderer::DrawMesh(Mesh* mesh, Material* material, const glm::mat4& modelMatrix)
    {
        // material->Bind();
        // Shader* shader = material->GetShader();
        // shader->SetMat4("u_Model", modelMatrix);
        // if (material->isLit) {
        //     shader->ApplyLightUniforms(lightsToRender);
        // }
        // else {
        //     shader->SetInt("u_IsLit", false);
        // }

        // mesh->GetVA().Bind();
        // GLCall(glDrawElements(GL_TRIANGLES, mesh->GetIB().GetCount(), GL_UNSIGNED_INT, 0));
        // mesh->GetVA().Unbind();
        // // mesh->Draw();
        // material->UnBind();
    }

    glm::mat4 Renderer::GetViewMatrix()
    {
        return viewMatrix;
    }

    glm::mat4 Renderer::GetProjectionMatrix()
    {
        return projectionMatrix;
    }

    glm::vec3 Renderer::GetCameraPosition()
    {
        return cameraPosition;
    }

    void Renderer::SetViewProjection(glm::mat4 view, glm::mat4 projection, glm::vec3 camPosition)
    {
        viewMatrix = view;
        projectionMatrix = projection;
        cameraPosition = camPosition;
    }
}