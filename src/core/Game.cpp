#include "Game.h"
#include "Renderer.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

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

    scene = new Scene();
    SetupScene();

    uiManager = new UI(window);
}

void Game::SetupScene() {
    // Setup camera (perspective, position, etc.)
    camera = new Camera(45.0f, (float)windowWidth / windowHeight, 0.1f, 100.0f);
    scene->AddCamera("MainCamera", camera);
    scene->SetActiveCamera("MainCamera");

    // Initialize some game objects, load models, etc.
    Model* model = new Model();
    model->LoadGLTF("assets/models/monkey.gltf");

    Shader* shader = new Shader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");

    for (Mesh& mesh : model->GetMeshes()) {
        if (mesh.GetMaterial()) {
            mesh.GetMaterial()->SetShader(shader);
        }
    }

    // Create GameObject(s)
    GameObject* obj1 = new GameObject();
    obj1->SetModel(model);  // Set the loaded model
    // obj1->SetScale(glm::vec3(1.0, 2.0, 1.0));
    obj1->SetPosition(glm::vec3(-2.0f, 0.0f, -5.0f));
    obj1->SetRotation(glm::quat(glm::radians(glm::vec3(-45.0, 0.0, 0))));

    GameObject* obj2 = new GameObject();
    obj2->SetModel(model);  // Set the same model or a different one
    obj2->SetPosition(glm::vec3(2.0f, 0.0f, -5.0f));

    scene->AddGameObject(obj1);
    scene->AddGameObject(obj2);

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
    // Other input handling (camera movement, etc.) can be added here.
}

void Game::Update(float deltaTime) {
    scene->Update(deltaTime);
}

void Game::Render() {
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f); // Clear screen color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Renderer::RenderScene(*scene, *scene->GetActiveCamera());
    
    uiManager->StartFrame();
    uiManager->ShowGameObjectEditor(scene);
    uiManager->Render();

}
