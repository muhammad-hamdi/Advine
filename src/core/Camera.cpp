#include "Camera.h"

#include "core/Input.h"

#include <iostream>

namespace Engine {
    Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane) {
        projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);

        // Default values
        position = glm::vec3(0.0f, 0.0f, 3.0f);
        front = glm::vec3(0.0f, 0.0f, -1.0f);
        up = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    glm::mat4 Camera::GetViewMatrix() const
    {
        return glm::lookAt(position, position + front, up);
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

    glm::vec3 Camera::GetPosition()
    {
        return position;
    }

    void Camera::SetTarget(const glm::vec3& t) {
        front = t;
    }

    void Camera::SetUp(const glm::vec3& u) {
        up = u;
    }

    void Camera::Update(float deltaTime)
    {
        if (Input::IsMouseCaptured()) {
            auto mousePos = Input::GetMouseDelta();
            yaw += mousePos.x * 0.1f;
            pitch -= mousePos.y * 0.1f;
            pitch = glm::clamp(pitch, -89.0f, 89.0f);

            glm::vec3 direction;
            direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            direction.y = sin(glm::radians(pitch));
            direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            direction = glm::normalize(direction);
            front = direction;
        }

        glm::vec3 right = glm::normalize(glm::cross(front, up));
        if (Input::IsKeyPressed(GLFW_KEY_W)) {
            position += front * 0.1f;
        }
        if (Input::IsKeyPressed(GLFW_KEY_S)) {
            position -= front * 0.1f;
        }

        if (Input::IsKeyPressed(GLFW_KEY_A)) {
            position -= right * 0.1f;
        }
        if (Input::IsKeyPressed(GLFW_KEY_D)) {
            position += right * 0.1f;
        }
    }
}