#pragma once
#include "core/Component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CameraComponent : public Component {
public:
    float fov = 60.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;
    glm::mat4 projectionMatrix;

    bool isActive = false; // helpful if you support multiple cameras

    glm::mat4 GetProjectionMatrix(float aspectRatio) const {
        return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }

    glm::mat4 GetViewMatrix(const glm::vec3& position, const glm::vec3& forward, const glm::vec3& up) const {
        return glm::lookAt(position, position + forward, up);
    }
};