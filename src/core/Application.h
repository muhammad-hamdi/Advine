#pragma once

namespace Engine
{
    struct Context {
        // AssetManager* assetManager;
        // NRenderer* renderer;
        // ::GLFWWindow* window;
        // Scene* scene;
    };

    class Application
    {
    private:
        // AssetManager mAssetManager;
        // NRenderer mRenderer;
        // ::GLFWWindow mWindow;
        // Scene mScene;
        Context mContext;
    public:
        Application(/* args */);
        ~Application();
    };
} // namespace Engine
