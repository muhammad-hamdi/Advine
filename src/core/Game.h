#pragma once
#include "Scene.h"
#include "Camera.h"
#include "UI.h"
#include "core/GameObject.h"
#include <GLFW/glfw3.h>

class Game {
public:
    Game(int windowWidth, int windowHeight, const char* windowTitle);
    ~Game();

    void Run();
    void AddObjectToScene(GameObject* object);
    void SetActiveCamera(const std::string& name);

private:
    void Initialize();
    void SetupScene();
    void Update(float deltaTime);
    void Render();
    void ProcessInput(float deltaTime);

    GLFWwindow* window;
    Scene* scene;
    Camera* camera;
    UI* uiManager;

    inline static int windowWidth;
    inline static int windowHeight;

    int viewportWidth;

    static void ResizeCallback(GLFWwindow* window, int width, int height);

    float fps = 0;
};
