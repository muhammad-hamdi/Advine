#include "Game.h"

#include "core/Input.h"
#include "core/Entity.h"
#include "core/Context.h"

#include "rendering/Renderer.h"
#include "assets/AssetManager.h"

#include "components/Camera.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <iostream>

namespace Engine {
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

        Context::Get().SetRenderer(&mRenderer);

        uiManager = new UI(window);
        editorCamera = new Camera(60.0f, (float)windowWidth / (float)windowHeight, 0.1f, 1000.0f);
        SetupScene();
    }

    void Game::SetupScene() {
        scene = new Scene("assets/scenes/Testing_Scene.json");
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
        if (Input::IsKeyPressed(GLFW_KEY_Q)) {
            Input::SetMouseCaptured(false);
        }
    }

    void Game::ResizeCallback(::GLFWwindow* window, int width, int height)
    {
        windowWidth = width;
        windowHeight = height;
        glViewport(0, 0, width, height);
    }

    void Game::Update(float deltaTime) {
        fps = 1 / deltaTime;
        if (state == State::Editor) {
            editorCamera->Update(deltaTime);
            if (windowHeight > 0) {
                editorCamera->SetProjectionMatrix(45.0f, (float)windowWidth / windowHeight, 0.1f, 100.0f);
                Renderer::SetViewProjection(editorCamera->GetViewMatrix(), editorCamera->GetProjectionMatrix(), editorCamera->GetPosition());
            }
        }
        else if (state == State::InGame) {
            scene->Update(deltaTime);

            Entity* activeCameraEntity = scene->GetActiveCameraEntity();
            if (activeCameraEntity) {
                auto* cam = activeCameraEntity->GetComponent<CameraComponent>();
                glm::mat4 view = cam->GetViewMatrix(
                    activeCameraEntity->GetWorldPosition(),
                    activeCameraEntity->transform.GetForwardDirection(),
                    glm::vec3(0.0f, 1.0f, 0.0f)
                );
                if (windowHeight > 0) {
                    glm::mat4 projection = cam->GetProjectionMatrix((float)windowWidth / (float)windowHeight);
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
}