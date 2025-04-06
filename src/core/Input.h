#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <vector>

enum class InputAction {
    Jump,
    Shoot,
    Sprint,
    Pause,
    // Add more as needed
};

enum class InputContext {
    Gameplay,
    UI,
    // Add more if needed
};

class Input {
public:
    static void Init(GLFWwindow* window);
    static void Update(); // Call once per frame

    // Raw input
    static bool IsKeyPressed(int key);
    static bool IsMousePressed(int button);
    static glm::vec2 GetMousePosition();
    static glm::vec2 GetMouseDelta();

    static void ResetMousePos();
    static void SetMouseCaptured(bool captured);
    static bool IsMouseCaptured();

    // Actions
    static bool IsActionPressed(InputAction action);
    static bool WasActionJustPressed(InputAction action);
    static bool WasActionJustReleased(InputAction action);

    static void SetContext(InputContext context);
    static void BindAction(InputAction action, int key); // Simple key binding

private:
    static GLFWwindow* s_Window;
    static std::unordered_map<InputAction, std::vector<int>> s_Bindings;
    static std::unordered_map<int, bool> s_PreviousKeyState;
    static InputContext s_CurrentContext;
    static inline glm::vec2 s_CurrentMousePos = { 0.0f, 0.0f };
    static inline glm::vec2 s_LastMousePos = { 0.0f, 0.0f };
    static inline bool s_MouseCaptured = false;
};
