#ifndef RENDERER_H
#define RENDERER_H

#include "Scene.h"
#include "Camera.h"

class Renderer {
public:
    static void RenderScene(Scene& scene, Camera& camera);
private:
    static void RenderGameObject(GameObject* object, const Camera& camera, glm::mat4 parentTransform);
};

#endif
