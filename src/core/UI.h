#pragma once
#include "Scene.h"

struct GLFWwindow;

class UI {
    public:
    UI(GLFWwindow* window);
    ~UI();

    void ShowGameObjectEditor(Scene* scene);
    void StartFrame();
    void Render();
};