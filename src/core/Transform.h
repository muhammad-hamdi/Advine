#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
public:
    //Local space information
    glm::vec3 position = { 0.0f, 0.0f, 0.0f };
    glm::vec3 eulerRotation = { 0.0f, 0.0f, 0.0f }; //In degrees
    glm::quat rotation = { 1.0f, 0.0f, 0.0f, 0.0f };
    glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

private:
    //Global space information concatenate in matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    //Dirty flag
    bool dirty = true;

public:
    glm::mat4 GetLocalModelMatrix() const;
    glm::vec3 GetForwardDirection() const;
    glm::vec3 GetRightDirection() const;

    void computeModelMatrix();

    void computeModelMatrix(const glm::mat4& parentGlobalModelMatrix);

    void setLocalPosition(const glm::vec3& newPosition);

    const glm::vec3& getLocalPosition();

    const glm::vec3& getLocalRotation();

    void setLocalRotation(const glm::vec3& newRotation);

    const glm::vec3& getLocalScale();

    void setLocalScale(const glm::vec3& newScale);

    const glm::mat4& getModelMatrix();

    bool IsDirty();
};
