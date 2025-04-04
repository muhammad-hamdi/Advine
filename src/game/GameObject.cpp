// GameObject.cpp
#include "GameObject.h"
#include "graphics/Model.h"
#include "graphics/Mesh.h"
#include "graphics/Material.h"
#include "graphics/Shader.h"

void GameObject::SetModel(Model* m) {
    model = m;
}

void GameObject::Update(float deltaTime) {
    // glm::vec3 movement = glm::vec3(0.1f * deltaTime, 0.0f, 0.0f); // Move along X-axis for example

    // Apply movement to the existing transformation matrix by modifying the translation part
    // transform = glm::translate(transform, movement);
}

void GameObject::Render(const Camera& camera, glm::mat4 globalTransform) {
    if (!model) return;

    glm::mat4 modelMatrix = globalTransform; // Use the combined global transform

    for (Mesh& mesh : model->GetMeshes()) {
        Material* material = mesh.GetMaterial();
        Shader* shader = material->GetShader();
        shader->Use();

        // Set common uniforms
        shader->SetMat4("model", modelMatrix);
        shader->SetMat4("view", camera.GetViewMatrix()); // Get view matrix from camera
        shader->SetMat4("projection", camera.GetProjectionMatrix()); // Get projection matrix from camera

        // Bind material (textures, etc.)
        material->Bind();

        // Draw mesh
        mesh.Draw();
    }
}

void GameObject::SetPosition(const glm::vec3& position) {
    transform = glm::translate(glm::mat4(1.0f), position) * transform;
}

void GameObject::SetRotation(const glm::quat& rotation) {
    transform = transform * glm::mat4_cast(rotation);
}

void GameObject::SetScale(const glm::vec3& scale) {
    transform = glm::scale(glm::mat4(1.0f), scale) * transform;
}