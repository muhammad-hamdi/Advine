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

void GameObject::SetMaterial(Material *material)
{
    globalMaterial = material;
}

void GameObject::Update(float deltaTime)
{
}
static int counter = 0;
void GameObject::Render(const Camera& camera, glm::mat4 globalTransform) {
    counter++;
    if (!model) return;

    glm::mat4 modelMatrix = globalTransform; // Use the combined global transform

    for (Mesh& mesh : model->GetMeshes()) {
        Material* material = GetMaterialForMesh(&mesh);
        if(!material) continue;

        Shader* shader = material->GetShader();
        material->Bind();
        shader->SetFloat("time", (float)counter);
        // Set common uniforms
        shader->SetMat4("model", modelMatrix);
        shader->SetMat4("view", camera.GetViewMatrix());
        shader->SetMat4("projection", camera.GetProjectionMatrix());
        // Draw mesh
        mesh.Draw();
        material->UnBind();
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

void GameObject::AddMaterialOverride(Mesh *mesh, Material *material)
{
    materialOverrides[mesh] = material;
}

Material *GameObject::GetMaterialForMesh(Mesh *mesh)
{
    if (materialOverrides.find(mesh) != materialOverrides.end()) {
        return materialOverrides[mesh];
    }
    if (globalMaterial) return globalMaterial;
    return mesh->GetMaterial(); // default fallback
}

std::string GameObject::GetName() const
{
    return name;
}

std::vector<GameObject *> GameObject::GetChildren() const
{
    return children;
}
