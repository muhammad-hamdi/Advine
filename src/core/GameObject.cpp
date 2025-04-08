// GameObject.cpp
#include "GameObject.h"
#include "assets/Model.h"
#include "assets/Shader.h"
#include "rendering/Mesh.h"
#include "rendering/Material.h"

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
void GameObject::Render(const Camera& camera, glm::mat4 accumalatedTransform) {
    counter++;
    if (!model) return;

    glm::mat4 modelMatrix = accumalatedTransform;

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

void GameObject::SetTransform(const glm::mat4 &newTransform)
{
    // transform.setLocalPosition(newTransform);
}

void GameObject::SetPosition(const glm::vec3 &newPosition)
{
    position = newPosition;
}

void GameObject::SetRotation(const glm::vec3& newRotation) {
    rotation = newRotation;
}

void GameObject::SetScale(const glm::vec3& newScale) {
    scale = newScale;
}

glm::mat4 GameObject::GetTransform() const {
    // Compute the transform matrix by combining position, rotation, and scale
    // glm::mat4 transform = glm::mat4(1.0f);
    // transform = glm::translate(transform, position);  // Apply translation (position)
    // transform *= glm::mat4_cast(rotation);  // Apply rotation (using quaternion)
    // transform = glm::scale(transform, scale);  // Apply scaling

    const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f),
                    glm::radians(rotation.x),
                    glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f),
                glm::radians(rotation.y),
                glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f),
                glm::radians(rotation.z),
                glm::vec3(0.0f, 0.0f, 1.0f));

    const glm::mat4 roationMatrix = transformY * transformX * transformZ;
    return glm::translate(glm::mat4(1.0f), position) * roationMatrix * glm::scale(glm::mat4(1.0f), scale);
}

glm::vec3 GameObject::GetPosition() const
{
    return position;
}

glm::vec3 GameObject::GetRotation() const
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
