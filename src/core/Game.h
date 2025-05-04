#pragma once
#include "core/Scene.h"
#include "core/Camera.h"
#include "core/UI.h"

#include "render/NRenderer.h"
#include "render/SceneRenderer.h"

#include <GLFW/glfw3.h>

namespace Engine {
    enum State {
        Editor,
        InGame
    };

    class Game {
    private:
        float fps = 0;
        std::string windowTitle;
        NRenderer mRenderer;
        SceneRenderer mSceneRenderer;

        ::GLFWwindow* window;
        Scene* scene;
        Camera* editorCamera;
        UI* uiManager;
    public:
        Game(int windowWidth, int windowHeight, const char* windowTitle);
        ~Game();

        void Run();
        void SetActiveCamera(const std::string& name);
        inline static int state = State::Editor;

        inline static int windowWidth;
        inline static int windowHeight;
    private:
        void Initialize();
        void SetupScene();
        void Update(float deltaTime);
        void Render();
        void ProcessInput(float deltaTime);

        static void ResizeCallback(::GLFWwindow* window, int width, int height);
    };
}