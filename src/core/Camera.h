#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(float fov, float aspectRatio, float nearPlane, float farPlane);

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
     // Method to set a new projection matrix
    void SetProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane);

    void SetPosition(const glm::vec3& pos);
    glm::vec3 GetPosition();
    void SetTarget(const glm::vec3& target);
    void SetUp(const glm::vec3& up);

    void Update(float deltaTime);

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    glm::mat4 projectionMatrix;

    float yaw = -90.0f;
    float pitch = 0;
};

#endif