#include "core/Transform.h"
#include "Transform.h"

glm::mat4 Transform::GetLocalModelMatrix() const
{
    const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f),
        glm::radians(eulerRotation.x),
        glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f),
        glm::radians(eulerRotation.y),
        glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f),
        glm::radians(eulerRotation.z),
        glm::vec3(0.0f, 0.0f, 1.0f));

    // Y * X * Z
    const glm::mat4 roationMatrix = transformY * transformX * transformZ;

    // translation * rotation * scale (also know as TRS matrix)
    return glm::translate(glm::mat4(1.0f), position) *
        roationMatrix *
        glm::scale(glm::mat4(1.0f), scale);
}

glm::vec3 Transform::GetForwardDirection() const {
    glm::vec3 direction;
    direction.x = cos(rotation.y) * cos(rotation.x);
    direction.y = sin(rotation.x);
    direction.z = sin(rotation.y) * cos(rotation.x);
    return glm::normalize(direction);
}

glm::vec3 Transform::GetRightDirection() const {
    return glm::normalize(glm::cross(GetForwardDirection(), glm::vec3(0.0f, 1.0f, 0.0f)));
}

void Transform::computeModelMatrix()
{
    modelMatrix = GetLocalModelMatrix();
    dirty = false;
}

void Transform::computeModelMatrix(const glm::mat4& parentGlobalModelMatrix)
{
    modelMatrix = parentGlobalModelMatrix * GetLocalModelMatrix();
    dirty = false;
}

void Transform::setLocalPosition(const glm::vec3& newPosition)
{
    position = newPosition;
    dirty = true;
}

const glm::vec3& Transform::getLocalPosition()
{
    return position;
}

const glm::vec3& Transform::getLocalRotation()
{
    return eulerRotation;
}

void Transform::setLocalRotation(const glm::vec3& newRotation)
{
    eulerRotation = newRotation;
    dirty = true;
}

const glm::vec3& Transform::getLocalScale()
{
    return scale;
}

void Transform::setLocalScale(const glm::vec3& newScale)
{
    scale = newScale;
    dirty = true;
}

const glm::mat4& Transform::getModelMatrix()
{
    return modelMatrix;
}

bool Transform::IsDirty()
{
    return dirty;
}
