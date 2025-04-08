#include "Entity.h"

glm::mat4 Entity::GetWorldMatrix() const
{
    if (parent) {
        return parent->transform.GetLocalModelMatrix() * transform.GetLocalModelMatrix();
    } else {
        return transform.GetLocalModelMatrix();
    }
}

glm::vec3 Entity::GetWorldPosition() const
{
    return glm::vec3(GetWorldMatrix()[3]);
}

void Entity::Update(float deltaTime)
{
    for (const auto& comp : components) {
        comp.get()->Update(deltaTime);
    }
}
