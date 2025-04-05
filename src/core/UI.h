#pragma once
#include "Scene.h"

struct GLFWwindow;

class UI {
    public:
    UI(GLFWwindow* window);
    ~UI();

    void ShowGameObjectEditor(Scene* scene);
    void ShowFPS(float fps);
    void StartFrame();
    void Render();
};