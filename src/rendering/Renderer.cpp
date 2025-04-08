#include "Renderer.h"
#include "components/MeshRenderer.h"

void Renderer::RenderScene(Scene& scene, Camera& camera) {
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto& obj : scene.GetGameObjects()) {
        RenderGameObject(obj, camera, glm::mat4(1.0f));
    }
}

void Renderer::RenderScene(Scene &scene)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    lightsToRender.clear();
    scene.GatherLights(lightsToRender);

    for(auto& entity : scene.GetEntities()) {
        RenderEntity(entity);
    }
}

void Renderer::RenderEntity(Entity *entity) {
    MeshRenderer* mr = entity->GetComponent<MeshRenderer>();
    if(mr)
        for(int i = 0; i < mr->meshes.size(); i++) {
            DrawMesh(mr->meshes[i], mr->materials[i], entity->GetWorldMatrix());
        }
    for(auto& child : entity->children) {
        RenderEntity(child);
    }
}


void Renderer::DrawMesh(Mesh *mesh, Material *material, const glm::mat4 &modelMatrix)
{
    material->Bind();
    Shader* shader = material->GetShader();
    shader->SetMat4("u_Model", modelMatrix);
    shader->ApplyLightUniforms(lightsToRender);

    glBindVertexArray(mesh->GetVAO());
    glDrawElements(GL_TRIANGLES, mesh->GetIndices().size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    material->UnBind();
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

void Renderer::RenderGameObject(GameObject *object, const Camera &camera, glm::mat4 parentTransform)
{
    if (!object) return;

    // If the object should be rendered in world space, we pass an identity matrix as the parent transform
    glm::mat4 globalTransform = parentTransform * object->GetTransform();

    // Check if the object is intended to be rendered in world space (this flag can be added to GameObject)
    if (object->isWorldSpace) {
        globalTransform = object->GetTransform();  // Ignore parent transform for world space objects
    }

    // Render the current object
    object->Render(camera, globalTransform);

    // Render all child objects recursively
    for (auto& child : object->children) {
        RenderGameObject(child, camera, globalTransform);
    }
}
