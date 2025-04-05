// GameObject.cpp
#include "GameObject.h"
#include "graphics/Model.h"
#include "graphics/Mesh.h"
#include "graphics/Material.h"
#include "graphics/Shader.h"

int GameObject::objectCount = 0;

GameObject::GameObject() {
    GameObject::objectCount++;
    name = "GameObject_" + std::to_string(GameObject::objectCount);
}

void GameObject::SetModel(Model* m) {
    model = m;
}

void GameObject::Update(float deltaTime)
{
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

void GameObject::SetPosition(const glm::vec3& newPosition) {
    position = newPosition;
}

void GameObject::SetRotation(const glm::quat& newRotation) {
    rotation = newRotation;
}

void GameObject::SetScale(const glm::vec3& newScale) {
    scale = newScale;
}

glm::mat4 GameObject::GetTransform() const {
    // Compute the transform matrix by combining position, rotation, and scale
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, position);  // Apply translation (position)
    transform *= glm::mat4_cast(rotation);  // Apply rotation (using quaternion)
    transform = glm::scale(transform, scale);  // Apply scaling
    return transform;
}

glm::vec3 GameObject::GetPosition() const
{
    return position;
}

glm::quat GameObject::GetRotation() const
{
    return rotation;
}

glm::vec3 GameObject::GetScale() const
{
    return scale;
}

std::string GameObject::GetName() const
{
    return name;
}
