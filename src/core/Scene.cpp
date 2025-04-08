#include "core/Scene.h"

#include "rendering/Renderer.h"
#include "components/Camera.h"

#include <algorithm>
#include <iostream>
#include "Scene.h"

Scene::Scene() {
    // Optionally, you can initialize a default camera or other scene components.
}

Scene::~Scene() {
    // Cleanup if necessary
}

void Scene::AddGameObject(GameObject* obj) {
    gameObjects.push_back(obj);
}

void Scene::RemoveGameObject(GameObject* obj) {
    auto it = std::remove(gameObjects.begin(), gameObjects.end(), obj);
    if (it != gameObjects.end()) {
        gameObjects.erase(it);
    }
}

void Scene::AddCamera(const std::string& name, Camera* camera) {
    cameras[name] = camera;
}

void Scene::SetActiveCamera(Entity *entity)
{
    entity->GetComponent<CameraComponent>()->isActive = true;
    activeCameraEntity = entity;
}

Entity *Scene::GetActiveCameraEntity() const
{
    return activeCameraEntity;
}

// Set the active camera by name
void Scene::SetActiveCamera(const std::string& name) {
    if (cameras.find(name) != cameras.end()) {
        activeCamera = cameras[name];
    }
}

Camera *Scene::GetActiveCamera() const
{
    return activeCamera;
}

void Scene::Update(float deltaTime)
{
    // Here, we could update GameObjects for things like physics, animation, etc.
    for (const auto& obj : gameObjects) {
        // For each GameObject, we can update its state
        // For example, applying movement, handling input, etc.
        obj->Update(deltaTime);
    }
}

const std::vector<GameObject*>& Scene::GetGameObjects() const {
    return gameObjects;
}

const std::vector<Entity *> &Scene::GetEntities() const
{
    return entities;
}
