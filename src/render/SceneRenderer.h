#pragma once

#include "core/Scene.h"

namespace Engine
{
    class Entity;
    class NRenderer;
    class SceneRenderer
    {
    private:
        NRenderer* renderer;
        std::vector<LightData> lightsToRender;
    public:
        SceneRenderer(NRenderer* renderer);
        ~SceneRenderer() = default;

        void Render(Scene* scene);
        void RenderEntity(Entity* entity);
    };
} // namespace Engine
