#include "UI.h"
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
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

    ImGui::Begin("Scene Tree");
    if(ImGui::Button("Deselect")) {
        selectedObject = nullptr;
    }
    if(ImGui::Button("New Object")) {
        printf("New Object\n");
    }
    for (auto& obj : gameObjects) {
        DrawObjectTree(obj);
    }
    ImGui::End();

    ImGui::Begin("Properties");
    if(selectedObject != nullptr) {
        ImGui::Text("Name");
        ImGui::SameLine();
        ImGui::InputText("##NameInput", &selectedObject->name);
        ImGui::Checkbox("World Transofrm", &selectedObject->isWorldSpace);
        if (ImGui::TreeNode("Transform")) {
            // Show the properties of the selected game selectedObjectect
            // Display the position
            glm::vec3 position = selectedObject->GetPosition();
            if (ImGui::DragFloat3("Position", &position[0])) {
                selectedObject->SetPosition(position);
            }

            // Display the rotation
            glm::vec3 rotation = glm::degrees(glm::eulerAngles(selectedObject->GetRotation()));  // Convert quat to Euler angles
            if (ImGui::DragFloat3("Rotation", &rotation[0])) {
                selectedObject->SetRotation(glm::quat(glm::radians(rotation)));
            }

            // Display the scale
            glm::vec3 scale = selectedObject->GetScale();
            if (ImGui::DragFloat3("Scale", &scale[0])) {
                selectedObject->SetScale(scale);
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Material")) {
            if(selectedObject->model) {
                for(auto mesh: selectedObject->model->GetMeshes()) {
                    auto mat = selectedObject->GetMaterialForMesh(&mesh);
                    ImGui::Text("Name:");
                    ImGui::SameLine();
                    ImGui::Text(mat->name.c_str());
                }
            }
            ImGui::TreePop();
        }
    } else {
        ImGui::Text("Select a GameObject");
    }

    ImGui::End();
}

void UI::DrawObjectTree(GameObject* n)
{
    int flags = ImGuiTreeNodeFlags_SpanFullWidth;
    if(n->GetChildren().size() == 0) {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }
    if(n == selectedObject) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }
    if (ImGui::TreeNodeEx(n->GetName().c_str(), flags)) {
        if (ImGui::IsItemClicked())
        {
            selectedObject = n;
        }
        for (auto child: n->GetChildren())
            DrawObjectTree(child);
        ImGui::TreePop();
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
