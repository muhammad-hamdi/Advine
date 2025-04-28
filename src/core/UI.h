#pragma once
#include "core/Scene.h"
#include "core/Entity.h"

struct GLFWwindow;

namespace Engine {
    class Game;

    class UI {
    public:
        UI(::GLFWwindow* window);
        ~UI();

        static const int imguiPanelWidth = 300;
        GameObject* selectedObject = nullptr;
        Entity* selectedEntity = nullptr;

        void ShowFPS(float fps);
        void StartFrame();
        void Render();

        void ShowGameObjectEditor(Scene* scene);
        void DrawObjectTree(GameObject* n, int id);
        void DrawSceneGraph(Entity* n, int id);

    private:
        ::GLFWwindow* window;
    };
}