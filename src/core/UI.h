#pragma once
#include "Scene.h"

struct GLFWwindow;

class UI {
    public:
    UI(GLFWwindow* window);
    ~UI();

    GameObject* selectedObject = nullptr;

    void ShowFPS(float fps);
    void StartFrame();
    void Render();

    void ShowGameObjectEditor(Scene* scene);
    void DrawObjectTree(GameObject* n, int id);
};