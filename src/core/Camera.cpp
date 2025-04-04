#include "Camera.h"

Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane) {
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);

    // Default values
    position = glm::vec3(0.0f, 0.0f, 3.0f);
    target = glm::vec3(0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(position, target, up);
}

glm::mat4 Camera::GetProjectionMatrix() const {
    return projectionMatrix;
}

 // Method to set a new projection matrix
void Camera::SetProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane) {
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Camera::SetPosition(const glm::vec3& pos) {
    position = pos;
}

void Camera::SetTarget(const glm::vec3& t) {
    target = t;
}

void Camera::SetUp(const glm::vec3& u) {
    up = u;
}
