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
    direction.x = cos(glm::radians(eulerRotation.y)) * cos(glm::radians(eulerRotation.x));
    direction.y = sin(glm::radians(eulerRotation.x));
    direction.z = sin(glm::radians(eulerRotation.y)) * cos(glm::radians(eulerRotation.x));
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

const glm::vec3& Transform::getLocalPosition()
{
    return position;
}

const glm::vec3& Transform::getLocalRotation()
{
    return eulerRotation;
}

const glm::vec3& Transform::getLocalScale()
{
    return scale;
}

void Transform::setLocalPosition(const glm::vec3& newPosition)
{
    position = newPosition;
    positionOverridden = true;
}

void Transform::setLocalRotation(const glm::vec3& newRotation)
{
    eulerRotation = newRotation;
    rotationOverridden = true;
}

void Transform::setLocalScale(const glm::vec3& newScale)
{
    scale = newScale;
    scaleOverridden = true;
}

const glm::mat4& Transform::getModelMatrix()
{
    return modelMatrix;
}

bool Transform::IsDirty()
{
    return dirty;
}
