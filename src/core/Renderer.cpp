#include "Renderer.h"

void Renderer::RenderScene(Scene& scene, Camera& camera) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto& obj : scene.GetGameObjects()) {
        RenderGameObject(obj, camera, glm::mat4(1.0f));
    }
}

void Renderer::RenderGameObject(GameObject* object, const Camera& camera, glm::mat4 parentTransform) {
    if (!object) return;

    // If the object should be rendered in world space, we pass an identity matrix as the parent transform
    glm::mat4 globalTransform = parentTransform * object->transform;

    // Check if the object is intended to be rendered in world space (this flag can be added to GameObject)
    if (object->isWorldSpace) {
        globalTransform = object->transform;  // Ignore parent transform for world space objects
    }

    // Render the current object
    object->Render(camera, globalTransform);

    // Render all child objects recursively
    for (auto& child : object->children) {
        RenderGameObject(child, camera, globalTransform);
    }
}
