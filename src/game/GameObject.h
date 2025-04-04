#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>
#include "graphics/Model.h"
#include "core/Camera.h"

class GameObject {
public:
    glm::mat4 transform;
    std::vector<GameObject*> children;
    Model* model = nullptr;
    bool isWorldSpace = false;  // Flag to indicate if the object should move relative to world

    void SetWorldSpace(bool flag) {
        isWorldSpace = flag;
    }

    GameObject()
        : transform(glm::mat4(1.0f)) {}

    void Update(float deltaTime);
    void Render(const Camera& camera, glm::mat4 globalTransform);

    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::quat& rotation);
    void SetScale(const glm::vec3& scale);

    void SetModel(Model* model);
};

#endif
