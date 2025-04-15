#include "UI.h"

#include "core/Game.h"
#include "components/Camera.h"
#include "components/MeshRenderer.h"
#include "components/LightComponent.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <algorithm>
#include <string>
#include <math.h>

UI::UI(GLFWwindow *window): window(window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable keyboard controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

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
bool findStringIC(const std::string & strHaystack, const std::string & strNeedle)
{
  auto it = std::search(
    strHaystack.begin(), strHaystack.end(),
    strNeedle.begin(),   strNeedle.end(),
    [](unsigned char ch1, unsigned char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
  );
  return (it != strHaystack.end() );
}

void DrawMaterialEditor(Material* material) {
    for (const auto& uniform : material->GetUniformMetadata()) {
        const std::string& name = uniform.name;

        // Only handle a few types for now
        if (uniform.type == GL_FLOAT) {
            float value = material->customUniforms[name].first.f;
            if (ImGui::DragFloat(name.c_str(), &value)) {
                material->SetCustomUniform(name, value, GL_FLOAT);
            }
        }
        else if (uniform.type == GL_INT) {
            int value = material->customUniforms[name].first.i;
            if (ImGui::DragInt(name.c_str(), &value)) {
                material->SetCustomUniform(name, value, GL_INT);
            }
        }
        else if (uniform.type == GL_FLOAT_VEC3) {
            glm::vec3 value = material->customUniforms[name].first.v3;
            if(findStringIC(name, "color")) {
                if (ImGui::ColorEdit3(name.c_str(), glm::value_ptr(value))) {
                    material->SetCustomUniform(name, value, GL_FLOAT_VEC3);
                }
            } else {
                if (ImGui::DragFloat3(name.c_str(), glm::value_ptr(value))) {
                    material->SetCustomUniform(name, value, GL_FLOAT_VEC3);
                }
            }
        }
        // Add other types as needed
    }
}

void UI::ShowGameObjectEditor(Scene *scene)
{
    // List all game objects in the scene
    std::vector<GameObject*> gameObjects = scene->GetGameObjects();

    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    int windowFlags = ImGuiWindowFlags_HorizontalScrollbar;

    ImGui::Begin("Entity Tree", NULL, windowFlags);

    ImGui::RadioButton("Editor", &Game::state, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Game", &Game::state, 1);

    if(ImGui::Button("Deselect")) {
        selectedObject = nullptr;
        selectedEntity = nullptr;
    }
    ImGui::SameLine();
    if(ImGui::Button("New Entity")) {
        Entity* entitiy = new Entity();
        entitiy->name = "New Entity";
        if(selectedEntity != nullptr) {
            selectedEntity->children.push_back(entitiy);
            entitiy->parent = selectedEntity;
        } else {
            scene->AddEntity(entitiy);
        }
    }
    ImGui::SameLine();
    if(ImGui::Button("Save Scene")) {
        scene->SaveToFile();
    }
    int id = 0;
    for (auto& obj : gameObjects) {
        DrawObjectTree(obj, id++);
    }

    for (auto& entity : scene->GetEntities()) {
        DrawSceneGraph(entity, id++);
    }
    ImGui::End();

    ImGui::Begin("Entity Inspector", NULL, windowFlags);
    if(selectedObject != nullptr) {
        ImGui::Text("Name");
        ImGui::SameLine();
        ImGui::InputText("##NameInput", &selectedObject->name);
        ImGui::Checkbox("World Transofrm", &selectedObject->isWorldSpace);
        if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            // Show the properties of the selected game selectedObjectect
            // Display the position
            glm::vec3 position = selectedObject->GetPosition();
            if (ImGui::DragFloat3("Position", &position[0])) {
                selectedObject->SetPosition(position);
            }

            // Display the rotation
            glm::vec3 rotation = selectedObject->GetRotation();  // Convert quat to Euler angles
            if (ImGui::DragFloat3("Rotation", &rotation[0])) {
                selectedObject->SetRotation(rotation);
            }

            // Display the scale
            glm::vec3 scale = selectedObject->GetScale();
            if (ImGui::DragFloat3("Scale", &scale[0])) {
                selectedObject->SetScale(scale);
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNodeEx("Material", ImGuiTreeNodeFlags_DefaultOpen)) {
            if(selectedObject->model) {
                for(auto mesh: selectedObject->model->GetMeshes()) {
                    auto mat = selectedObject->GetMaterialForMesh(&mesh);
                    ImGui::Text("Name:");
                    ImGui::SameLine();
                    ImGui::Text(mat->name.c_str());
                    DrawMaterialEditor(mat);
                }
            }
            ImGui::TreePop();
        }
    }

    if(selectedEntity != nullptr) {
        ImGui::Text("Name");
        ImGui::SameLine();
        ImGui::InputText("##NameInput", &selectedEntity->name);
        if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            // Display the position
            glm::vec3 position = selectedEntity->transform.getLocalPosition();
            if (ImGui::DragFloat3("Position", &position[0], 0.1f)) {
                selectedEntity->transform.setLocalPosition(position);
            }

            // Display the rotation
            glm::vec3 rotation = selectedEntity->transform.getLocalRotation();
            if (ImGui::DragFloat3("Rotation", &rotation[0], 0.1f)) {
                selectedEntity->transform.setLocalRotation(rotation);
            }

            // Display the scale
            glm::vec3 scale = selectedEntity->transform.getLocalScale();
            if (ImGui::DragFloat3("Scale", &scale[0], 0.1f)) {
                selectedEntity->transform.setLocalScale(scale);
            }
            ImGui::TreePop();
        }

        for(auto &comp : selectedEntity->components) {
            ImGui::Separator();
            if(CameraComponent* camera = dynamic_cast<CameraComponent*>(comp.get())) {
                // do camera stuff, maybe skybox cubemap
                if(ImGui::TreeNode("Camera Component")) {
                    ImGui::SliderFloat("fov", &camera->fov, 10.f, 179.f);
                    ImGui::Checkbox("Main Camera", &camera->isActive);
                }
            }
            else if(LightComponent* lc = dynamic_cast<LightComponent*>(comp.get())) {
                if(ImGui::TreeNode("Light Component")) {
                    char* types[3] = {
                        "Directional Light",
                        "Point Light",
                        "Spot Light"
                    };
                    if(ImGui::BeginCombo("Type", types[static_cast<int>(lc->type)])) {
                        for(int i = 0; i < 3; i++) {
                            auto type = types[i];
                            if(ImGui::Selectable(type, i == static_cast<int>(lc->type))) {
                                lc->type = static_cast<LightType>(i);
                            }
                        }
                        ImGui::EndCombo();
                    }
                    ImGui::ColorEdit3("Color", &lc->color[0]);
                    ImGui::SliderFloat("Intensity", &lc->intensity, 0.f, 1.0f);
                    if(lc->type != LightType::Directional) {
                        ImGui::Text("Attenuation Factors");
                        ImGui::SliderFloat("Constant", &lc->constant, 0.f, 1.0f);
                        ImGui::SliderFloat("Linear", &lc->linear, 0.f, 1.0f);
                        ImGui::SliderFloat("Quadratic", &lc->quadratic, 0.f, 1.0f);
                    }
                    if(lc->type == LightType::Spot) {
                        float spotAngle = glm::degrees(lc->spotAngle);
                        if(ImGui::DragFloat("Spot Angle", &spotAngle)) {
                            lc->spotAngle = glm::radians(spotAngle);
                        }
                    }
                    ImGui::TreePop();
                }
            }
            else if (MeshRenderer* mr = dynamic_cast<MeshRenderer*>(comp.get())) {
                if(ImGui::TreeNode("MeshRenderer")) {
                    for(int i = 0; i < mr->meshes.size(); i++) {
                        auto mat = mr->materials[i];
                        ImGui::Text("Material:");
                        ImGui::SameLine();
                        ImGui::Text(mat->name.c_str());
                        ImGui::Checkbox("Is Lit", &mat->isLit);
                        if(ImGui::TreeNode("Textures")) {
                            for(int i = 0; i < mat->diffuseTextures.size(); i++) {
                                auto tex = mat->diffuseTextures[i];
                                ImGui::InputText("Diffuse##" + i, &tex->path);
                                ImGui::Image(tex->ID, ImVec2(__max(tex->width/5, 256), __max(tex->height/5, 256)));
                            }
                            for(int i = 0; i < mat->specularTextures.size(); i++) {
                                auto tex = mat->specularTextures[i];
                                ImGui::InputText("Specular ##" + i, &tex->path);
                                ImGui::Image(tex->ID, ImVec2(__max(tex->width/5, 256), __max(tex->height/5, 256)));
                            }
                            ImGui::TreePop();
                        }
                    }
                    ImGui::TreePop();
                }
            }
        }

        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("component_popup");
        if (ImGui::BeginPopup("component_popup")) {
            ImGui::MenuItem("(demo menu)", NULL, false, false);
            // if (ImGui::MenuItem("MeshRenderer")) {}
            if (ImGui::MenuItem("Light")) {
                selectedEntity->AddComponent<LightComponent>(LightType::Point);
            }
            if (ImGui::MenuItem("Camera")) {
                selectedEntity->AddComponent<CameraComponent>();
            }
            ImGui::EndPopup();
        }
    }

    ImGui::End();
}

void UI::DrawObjectTree(GameObject* n, int id)
{
    int flags = ImGuiTreeNodeFlags_SpanFullWidth;
    if(n->GetChildren().size() == 0) {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }
    if(n == selectedObject) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }
    if (ImGui::TreeNodeEx((n->GetName() + "###node_" + std::to_string(id)).c_str(), flags)) {
        if (ImGui::IsItemClicked())
        {
            selectedObject = n;
        }
        for (auto child: n->GetChildren())
            DrawObjectTree(child, id++);
        ImGui::TreePop();
    }
}

void UI::DrawSceneGraph(Entity *n, int id)
{
    int flags = ImGuiTreeNodeFlags_SpanFullWidth;
    if(n->children.size() == 0) {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }
    if(n == selectedEntity) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }
    if (ImGui::TreeNodeEx((n->name + "###node_" + std::to_string(id)).c_str(), flags)) {
        if (ImGui::IsItemClicked())
        {
            selectedEntity = n;
        }
        for (auto child: n->children)
            DrawSceneGraph(child, id++);
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
    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}
