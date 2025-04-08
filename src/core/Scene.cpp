#include "core/Scene.h"

#include "rendering/LightData.h"
#include "rendering/Renderer.h"
#include "components/Camera.h"
#include "components/LightComponent.h"

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

Entity *Scene::CreateEntity(const std::string &name)
{
    Entity *entity = new Entity();
    entity->name = name;
    entities.push_back(entity);
    return entity;
}

void Scene::AddEntity(Entity *entity)
{
    entities.push_back(entity);
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
    // for (const auto& obj : gameObjects) {
    //     // For each GameObject, we can update its state
    //     // For example, applying movement, handling input, etc.
    //     obj->Update(deltaTime);
    // }

    for (const auto& entity : entities) {
        entity->Update(deltaTime);
    }
}

const std::vector<GameObject*>& Scene::GetGameObjects() const {
    return gameObjects;
}

const std::vector<Entity *> &Scene::GetEntities() const
{
    return entities;
}

void Scene::GatherLights(std::vector<LightData>& lightsOut) {
    for (Entity* entity : entities) {
        auto* light = entity->GetComponent<LightComponent>();
        if (!light) continue;

        LightData data;
        data.type = static_cast<int>(light->type);
        data.color = light->color * light->intensity;
        data.position = entity->GetWorldPosition();
        data.direction = entity->transform.GetForwardDirection();
        data.range = light->range;
        data.spotAngle = light->spotAngle;
        data.castShadows = light->castShadows;

        lightsOut.push_back(data);

        if (light->type == LightType::Directional && light->isMainDirectional)
            mainDirectionalLight = data;
    }
}
