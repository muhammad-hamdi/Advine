#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>
#include "graphics/Model.h"
#include "core/Camera.h"

class GameObject {
public:
    static int objectCount;

    std::string name;
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);  // Position of the object
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);  // Rotation of the object (identity quaternion)
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);  // Scale of the object

    std::vector<GameObject*> children;
    Model* model = nullptr;
    bool isWorldSpace = false;  // Flag to indicate if the object should move relative to world

    void SetWorldSpace(bool flag) {
        isWorldSpace = flag;
    }

    GameObject();

    void Update(float deltaTime);
    void Render(const Camera& camera, glm::mat4 globalTransform);

    void SetModel(Model* model);

    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::quat& rotation);
    void SetScale(const glm::vec3& scale);

    glm::mat4 GetTransform() const;  // Method to get the combined transform matrix

    // Getters for position, rotation, and scale
    glm::vec3 GetPosition() const;

    glm::quat GetRotation() const;

    glm::vec3 GetScale() const;

    std::string GetName() const;
};

#endif
