#pragma once

#include "render/api/RenderAPI.h"

#include "core/Scene.h"

namespace Engine
{
    class Entity;
    class NRenderer;
    class SceneRenderer
    {
    private:
        const int SHADOW_WIDTH = 4096;
        const int SHADOW_HEIGHT = 4096;
        GPUHandle depthFramebuffer;
        GPUHandle depthMap;
        glm::mat4 lightSpaceMatrix;

        NRenderer* renderer;
        std::vector<LightData> lightsToRender;
    public:
        SceneRenderer(NRenderer* renderer);
        ~SceneRenderer() = default;
        void Init();

        GPUHandle GetDepthMap() { return depthMap; }

        void Render(Scene* scene);
        void RenderEntity(Entity* entity);
        void RenderEntityWithShader(Entity *entity, GPUHandle shader);
    };
} // namespace Engine
