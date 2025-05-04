#pragma once
#include "core/Component.h"
#include "core/Input.h"
#include "core/Entity.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine {

    class CameraComponent : public Component {
    public:
        float fov = 60.0f;
        float nearPlane = 0.1f;
        float farPlane = 1000.0f;

        float yaw = -90.0f;
        float pitch = 0;

        glm::mat4 projectionMatrix;

        bool isActive = false;

        glm::mat4 GetProjectionMatrix(float aspectRatio) const {
            return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
        }

        glm::mat4 GetViewMatrix() const {
            return glm::lookAt(owner->GetWorldPosition(), owner->GetWorldPosition() + owner->transform.GetForwardDirection(), glm::vec3(0.0f, 1.0f, 0.0f));
        }

        void Update(float deltaTime) override {
            if (Input::IsMouseCaptured()) {
                auto mousePos = Input::GetMouseDelta();
                yaw += mousePos.x * 0.1f;
                pitch -= mousePos.y * 0.1f;
                pitch = glm::clamp(pitch, -89.0f, 89.0f);
            }
            owner->transform.eulerRotation = { pitch, yaw, 0 };

            glm::vec3 front = owner->transform.GetForwardDirection();
            glm::vec3 right = owner->transform.GetRightDirection();
            if (Input::IsKeyPressed(GLFW_KEY_W)) {
                owner->transform.position += front * 0.1f;
            }
            if (Input::IsKeyPressed(GLFW_KEY_S)) {
                owner->transform.position -= front * 0.1f;
            }

            if (Input::IsKeyPressed(GLFW_KEY_A)) {
                owner->transform.position -= right * 0.1f;
            }
            if (Input::IsKeyPressed(GLFW_KEY_D)) {
                owner->transform.position += right * 0.1f;
            }
        }

        bool IsOverridden() const {
            return true;
        }

        json Serialize() const {
            json j;
            j["name"] = "camera";
            j["isActive"] = isActive;

            return j;
        }
    };
}