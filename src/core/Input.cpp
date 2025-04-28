#include "Input.h"

namespace Engine {
    ::GLFWwindow* Input::s_Window = nullptr;
    std::unordered_map<InputAction, std::vector<int>> Input::s_Bindings;
    std::unordered_map<int, bool> Input::s_PreviousKeyState;
    InputContext Input::s_CurrentContext = InputContext::Gameplay;

    void Input::Init(::GLFWwindow* window) {
        s_Window = window;

        // Example bindings
        BindAction(InputAction::Jump, GLFW_KEY_SPACE);
        BindAction(InputAction::Shoot, GLFW_MOUSE_BUTTON_LEFT);
    }

    void Input::Update() {
        s_LastMousePos = s_CurrentMousePos;
        double x, y;
        glfwGetCursorPos(s_Window, &x, &y);
        s_CurrentMousePos = { static_cast<float>(x), static_cast<float>(y) };
        // Store previous key states for just-pressed/released
        for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key) {
            s_PreviousKeyState[key] = glfwGetKey(s_Window, key) == GLFW_PRESS;
        }
    }

    bool Input::IsKeyPressed(int key) {
        return glfwGetKey(s_Window, key) == GLFW_PRESS;
    }

    bool Input::IsMousePressed(int button) {
        return glfwGetMouseButton(s_Window, button) == GLFW_PRESS;
    }

    glm::vec2 Input::GetMousePosition() {
        return s_CurrentMousePos;
    }

    glm::vec2 Input::GetMouseDelta()
    {
        return s_CurrentMousePos - s_LastMousePos;
    }

    void Input::ResetMousePos()
    {
        int windowWidth, windowHeight;
        glfwGetWindowSize(s_Window, &windowWidth, &windowHeight);
        glfwSetCursorPos(s_Window, windowWidth / 2.0f, windowHeight / 2.0f);
    }

    void Input::SetMouseCaptured(bool captured)
    {
        s_MouseCaptured = captured;
        glfwSetInputMode(s_Window, GLFW_CURSOR, captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }

    bool Input::IsMouseCaptured()
    {
        return s_MouseCaptured;
    }

    bool Input::IsActionPressed(InputAction action) {
        if (s_Bindings.find(action) == s_Bindings.end()) return false;

        for (int key : s_Bindings[action]) {
            if (key < GLFW_MOUSE_BUTTON_LAST) {
                if (IsMousePressed(key)) return true;
            }
            else {
                if (IsKeyPressed(key)) return true;
            }
        }

        return false;
    }

    bool Input::WasActionJustPressed(InputAction action) {
        if (s_Bindings.find(action) == s_Bindings.end()) return false;

        for (int key : s_Bindings[action]) {
            bool current = IsKeyPressed(key);
            bool previous = s_PreviousKeyState[key];
            if (current && !previous) return true;
        }

        return false;
    }

    bool Input::WasActionJustReleased(InputAction action) {
        if (s_Bindings.find(action) == s_Bindings.end()) return false;

        for (int key : s_Bindings[action]) {
            bool current = IsKeyPressed(key);
            bool previous = s_PreviousKeyState[key];
            if (!current && previous) return true;
        }

        return false;
    }

    void Input::SetContext(InputContext context) {
        s_CurrentContext = context;
        // Can change bindings depending on context if needed
    }

    void Input::BindAction(InputAction action, int key) {
        s_Bindings[action].push_back(key);
    }
}