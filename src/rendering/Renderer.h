#ifndef RENDERER_H
#define RENDERER_H

#include "core/Scene.h"
#include "core/Camera.h"

class Renderer {
public:
    static void RenderScene(Scene& scene, Camera& camera);
    static void RenderScene(Scene& scene);
    static void DrawMesh(Mesh* mesh, Material* material, const glm::mat4& modelMatrix);

    static glm::mat4 GetViewMatrix();
    static glm::mat4 GetProjectionMatrix();
    static glm::vec3 GetCameraPosition();

    static void SetViewProjection(glm::mat4 view, glm::mat4 projection, glm::vec3 camPosition);
private:
    static void RenderGameObject(GameObject* object, const Camera& camera, glm::mat4 parentTransform);

    inline static glm::mat4 viewMatrix;
    inline static glm::mat4 projectionMatrix;
    inline static glm::vec3 cameraPosition;
};

#endif
