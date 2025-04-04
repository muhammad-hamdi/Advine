#pragma once
#include "Scene.h"
#include "Camera.h"
#include "game/GameObject.h"
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

    int windowWidth;
    int windowHeight;
};
