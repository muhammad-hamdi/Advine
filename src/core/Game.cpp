#include "Game.h"

#include "core/Input.h"
#include "rendering/Renderer.h"
#include "assets/AssetManager.h"
#include "core/Entity.h"
#include "components/Camera.h"
#include "components/MeshRenderer.h"
#include "components/LightComponent.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <iostream>

Game::Game(int winWidth, int winHeight, const char* windowTitle) 
    : windowTitle(windowTitle), window(nullptr), scene(nullptr), editorCamera(nullptr), uiManager(nullptr) {
    windowWidth = winWidth;
    windowHeight = winHeight;
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
    glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);

    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), nullptr, nullptr);
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

    glfwSetFramebufferSizeCallback(window, ResizeCallback);

    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);
    glEnable(GL_DEPTH_TEST);

    AssetManager::Init();
    Input::Init(window);

    uiManager = new UI(window);
    scene = new Scene();
    editorCamera = new Camera(60.0f, (float)windowWidth / (float)windowHeight, 0.1f, 1000.0f);
    SetupScene();
}

void Game::SetupScene() {
    scene = new Scene("assets/scenes/Testing_Scene.json");

#if 0
    Model* loader = new Model();

    Entity* sonicEntity = loader->LoadAssimp("assets/models/sonic_the_hedgehog_running/scene.gltf");
    auto sonicSkeleton = sonicEntity->children[0]->children[1];
    sonicSkeleton->transform.position = {5, 7, -5};
    sonicSkeleton->transform.setLocalRotation({-90, 0, 0});
    sonicSkeleton->transform.scale = glm::vec3(0.2f);

    Entity* monkeyEntity = loader->LoadAssimp("assets/models/monkey_textured.gltf");
    monkeyEntity->transform.position = {5, 0, -5};

    Entity* nanosuitEntity = loader->LoadAssimp("assets/models/nanosuit/nanosuit.obj");
    nanosuitEntity->transform.position = {-5, 0, -5};

    Entity* boxEntity = loader->LoadAssimp("assets/models/cube.gltf");
    boxEntity->transform.position = {0, -5, 0};
    boxEntity->transform.scale = {10, 0.2, 10};

    Entity* cameraEntity = scene->CreateEntity("MainCamera");
    cameraEntity->transform.position = {0, 8, 15};
    cameraEntity->transform.setLocalRotation({ 0, -90, 0 });
    cameraEntity->AddComponent<CameraComponent>()->isActive = true;
    scene->SetActiveCamera(cameraEntity);

    Entity* directionalLight = scene->CreateEntity("sun");
    directionalLight->AddComponent<LightComponent>(LightType::Directional);
    directionalLight->transform.setLocalRotation({-45.0f, -45.0f, 0.0});

    Entity* pointLight = loader->LoadAssimp("assets/models/ball.gltf");
    pointLight->name = "lamp";
    // scene->CreateEntity("lamp");
    auto lc = pointLight->AddComponent<LightComponent>(LightType::Point);
    lc->color = {1.0f, 1.0f, 0.0f};
    lc->linear = 0.09;
    lc->quadratic = 0.032;
    pointLight->transform.position = {0, 5, 0};
    auto mr = pointLight->GetComponent<MeshRenderer>();
    mr->materials[0]->SetShader(AssetManager::LoadShader("emissive", "assets/shaders/default.vert", "assets/shaders/emissive.frag"));
    printf("%s\n", mr->materials[0]->diffuseTextures[0]->path.c_str());

    scene->AddEntity(sonicEntity);
    scene->AddEntity(pointLight);
    scene->AddEntity(monkeyEntity);
    scene->AddEntity(nanosuitEntity);
    scene->AddEntity(boxEntity);
#endif
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
    Input::Update();
    if (Input::IsMousePressed(GLFW_MOUSE_BUTTON_RIGHT)) {
        Input::SetMouseCaptured(true);
    }
    if(Input::IsKeyPressed(GLFW_KEY_Q)) {
        Input::SetMouseCaptured(false);
    }
}

void Game::ResizeCallback(GLFWwindow *window, int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);
}

void Game::Update(float deltaTime) {
    fps = 1/deltaTime;
    if(state == State::Editor) {
        editorCamera->Update(deltaTime);
        if(windowHeight > 0) {
            editorCamera->SetProjectionMatrix(45.0f, (float)windowWidth/windowHeight, 0.1f, 100.0f);
            Renderer::SetViewProjection(editorCamera->GetViewMatrix(), editorCamera->GetProjectionMatrix(), editorCamera->GetPosition());
        }
    }
    else if(state == State::InGame) {
        scene->Update(deltaTime);

        Entity* activeCameraEntity = scene->GetActiveCameraEntity();
        if (activeCameraEntity) {
            auto* cam = activeCameraEntity->GetComponent<CameraComponent>();
            glm::mat4 view = cam->GetViewMatrix(
                activeCameraEntity->GetWorldPosition(),
                activeCameraEntity->transform.GetForwardDirection(),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
            if(windowHeight > 0) {
                glm::mat4 projection = cam->GetProjectionMatrix((float)windowWidth/(float)windowHeight);
                Renderer::SetViewProjection(view, projection, activeCameraEntity->GetWorldPosition());
            }
        }
    }
}

void Game::Render() {
    Renderer::RenderScene(*scene);

    uiManager->StartFrame();
    uiManager->ShowGameObjectEditor(scene);
    uiManager->Render();
}
