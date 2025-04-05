#include "UI.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

UI::UI(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable keyboard controls

    // Setup ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

UI::~UI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UI::ShowGameObjectEditor(Scene *scene)
{
    // List all game objects in the scene
    std::vector<GameObject*> gameObjects = scene->GetGameObjects();

    // TODOD: instead of showing them directly with all their transoforms, only show the name and clicking it should open another menu with all game object details

    if (ImGui::Begin("Game Object Editor")) {
        for (auto& obj : gameObjects) {
            if (ImGui::TreeNode(obj->GetName().c_str())) {
                // Show the properties of the selected game object
                // Display the position
                glm::vec3 position = obj->GetPosition();
                if (ImGui::DragFloat3("Position", &position[0])) {
                    obj->SetPosition(position);
                }

                // Display the rotation
                glm::vec3 rotation = glm::degrees(glm::eulerAngles(obj->GetRotation()));  // Convert quat to Euler angles
                if (ImGui::DragFloat3("Rotation", &rotation[0])) {
                    obj->SetRotation(glm::quat(glm::radians(rotation)));
                }

                // Display the scale
                glm::vec3 scale = obj->GetScale();
                if (ImGui::DragFloat3("Scale", &scale[0])) {
                    obj->SetScale(scale);
                }

                ImGui::TreePop();
            }
        }

        ImGui::End();
    }
}

void UI::ShowFPS(float fps)
{
    ImGui::Text("%f", fps);
}

void UI::StartFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UI::Render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
