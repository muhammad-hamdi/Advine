#include "Game.h"

#include "core/Input.h"
#include "core/Renderer.h"
#include "core/AssetManager.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <iostream>

Game::Game(int windowWidth, int windowHeight, const char* windowTitle) 
    : windowWidth(windowWidth), windowHeight(windowHeight), window(nullptr), scene(nullptr), camera(nullptr), uiManager(nullptr) {
    Initialize();
}

Game::~Game() {
    delete uiManager;
    delete scene;
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Game::Initialize() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(windowWidth, windowHeight, "Game", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // Enable vsync

    // Initialize OpenGL loader (Glad in this case)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL loader!" << std::endl;
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);

    AssetManager::Init();
    Input::Init(window);

    uiManager = new UI(window);
    scene = new Scene();
    SetupScene();
}

void Game::SetupScene() {
    // Setup camera (perspective, position, etc.)
    camera = new Camera(45.0f, (float)windowWidth / windowHeight, 0.1f, 100.0f, windowWidth, windowHeight);
    scene->AddCamera("MainCamera", camera);
    scene->SetActiveCamera("MainCamera");

    Model* box = AssetManager::LoadModel("box", "assets/models/cube.gltf");
    Model* model = AssetManager::LoadModel("monkey", "assets/models/monkey.gltf");
    Model* modelTextured = AssetManager::LoadModel("monkey_textured", "assets/models/monkey_textured.gltf");

    // Create GameObject(s)
    GameObject* obj1 = new GameObject();
    obj1->SetModel(box);  // Set the loaded model
    // obj1->SetScale(glm::vec3(1.0, 2.0, 1.0));
    obj1->SetPosition(glm::vec3(-2.0f, 0.0f, -5.0f));
    obj1->SetRotation(glm::quat(glm::radians(glm::vec3(-45.0, 0.0, 0))));
    obj1->SetMaterial(AssetManager::LoadMaterial("scroll_tex", "assets/shaders/default.vert", "assets/shaders/scrolling_tex.frag", "assets/textures/ss.png"));

    GameObject* obj2 = new GameObject();
    obj2->SetModel(modelTextured);  // Set the same model or a different one
    obj2->SetPosition(glm::vec3(2.0f, 0.0f, -5.0f));

    GameObject* obj3 = new GameObject();
    obj3->SetModel(modelTextured);
    obj3->SetPosition(glm::vec3(0.0f, 0.0f, -5.0f));
    obj3->SetMaterial(AssetManager::GetMaterial("scroll_tex"));

    scene->AddGameObject(obj1);
    scene->AddGameObject(obj2);
    scene->AddGameObject(obj3);

    // More objects can be added as needed.
}

void Game::AddObjectToScene(GameObject* object) {
    scene->AddGameObject(object);
}

void Game::SetActiveCamera(const std::string& name) {
    scene->SetActiveCamera(name);
}

void Game::Run() {
    float lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        ProcessInput(deltaTime);
        Update(deltaTime);
        Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Game::ProcessInput(float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    Input::Update();
    // Other input handling (camera movement, etc.) can be added here.
    if (Input::IsMousePressed(GLFW_MOUSE_BUTTON_RIGHT)) {
        Input::SetMouseCaptured(true); // Capture the mouse
    }
    if(Input::IsKeyPressed(GLFW_KEY_Q)) {
        Input::SetMouseCaptured(false);
    }
    scene->GetActiveCamera()->Update(deltaTime);
}

void Game::Update(float deltaTime) {
    fps = 1/deltaTime;
    scene->Update(deltaTime);
}

void Game::Render() {
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f); // Clear screen color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Renderer::RenderScene(*scene, *scene->GetActiveCamera());
    
    uiManager->StartFrame();
    uiManager->ShowFPS(fps);
    uiManager->ShowGameObjectEditor(scene);
    uiManager->Render();

}
