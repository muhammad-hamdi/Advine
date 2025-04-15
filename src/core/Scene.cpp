#include "core/Scene.h"

#include "rendering/LightData.h"
#include "rendering/Renderer.h"
#include "components/Camera.h"
#include "components/LightComponent.h"

#include "json.hpp"

#include <algorithm>
#include <iostream>
#include <fstream>
#include "Scene.h"

using json = nlohmann::json;

Scene::Scene() {
}

Scene::Scene(std::string path): filepath(path)
{
    LoadFromFile();
}

Scene::~Scene() {
}

void Scene::LoadFromFile() {
    std::ifstream f(filepath);
    json sceneData = json::parse(f);

    name = sceneData["name"];

    Model loader;

    for(auto m : sceneData["entities"]) {
        Entity* e;
        if(m.contains("modelPath")) {
            e = loader.LoadAssimp(m["modelPath"]);
            e->modelPath = m["modelPath"];
            e->name = m["name"];
            entities.push_back(e);
        } else {
            e = CreateEntity(m["name"]);
            e->source = 1;
        }
        if(m.contains("components")) {
            for(auto co : m["components"]) {
                if(co["name"] == "camera") {
                    auto cm = e->AddComponent<CameraComponent>();
                    if(co["isActive"]) {
                        SetActiveCamera(e);
                    }
                }
                else if (co["name"] == "light")
                {
                    auto lc = e->AddComponent<LightComponent>(static_cast<LightType>(co["type"]));
                    if(co.contains("color")) {
                        lc->color = {co["color"][0], co["color"][1], co["color"][2]};
                    }
                    if(co["type"] != 0) {
                        lc->constant  = co.contains("constant")  ? static_cast<float>(co["constant"])  : 0;
                        lc->linear    = co.contains("linear")    ? static_cast<float>(co["linear"])    : 0;
                        lc->quadratic = co.contains("quadratic") ? static_cast<float>(co["quadratic"]) : 0;
                    }
                }
            }
        }
        if(m.contains("position")) {
            auto pos = m["position"];
            if(m.contains("motionNode")) {
                std::vector<int> sn = m["motionNode"];
                Entity* me = e;
                for(int i : sn) {
                    me = me->children[i];
                }
                me->transform.setLocalPosition({pos[0], pos[1], pos[2]});
            } else {
                e->transform.setLocalPosition({pos[0], pos[1], pos[2]});
            }
        }
        if(m.contains("rotation")) {
            auto rot = m["rotation"];
            if(m.contains("motionNode")) {
                std::vector<int> sn = m["motionNode"];
                Entity* me = e;
                for(int i : sn) {
                    me = me->children[i];
                }
                me->transform.setLocalRotation({rot[0], rot[1], rot[2]});
            } else {
                e->transform.setLocalRotation({rot[0], rot[1], rot[2]});
            }
        }
        if(m.contains("scale")) {
            auto scale = m["scale"];
            if(m.contains("motionNode")) {
                std::vector<int> sn = m["motionNode"];
                Entity* me = e;
                for(int i : sn) {
                    me = me->children[i];
                }
                me->transform.setLocalScale({scale[0], scale[1], scale[2]});
            } else {
                e->transform.setLocalScale({scale[0], scale[1], scale[2]});
            }
        }
    }
}

json Scene::SerializeEntities(const std::vector<Entity*>& entities) {
    json arr = json::array();
    for (auto* e : entities) {
        json entityJson;
        entityJson["name"] = e->name;
        if(e->HasTransformOverride()) {
            auto t = e->transform;
            entityJson["position"] = {t.position.x, t.position.y, t.position.z};
            entityJson["rotation"] = {t.eulerRotation.x, t.eulerRotation.y, t.eulerRotation.z};
            entityJson["scale"]    = {t.scale.x, t.scale.y, t.scale.z};
        }

        if (!e->components.empty()) {
            entityJson["components"] = json::array();
            for (auto& c : e->components) {
                if(c->IsOverridden()) {
                    entityJson["components"].push_back(c->Serialize());
                }
            }
        }

        if(e->source == 0) {
            entityJson["modelPath"] = e->modelPath;
            std::vector<json> overrides;
            e->GatherOverrides(overrides, {});
            if (!overrides.empty()) {
                entityJson["overrides"] = overrides;
            }
        } else {
            if (!e->children.empty()) {
                entityJson["children"] = SerializeEntities(e->children);
            }
        }
        arr.push_back(entityJson);
    }
    return arr;
}

void Scene::SaveToFile() {
    std::string file = "assets/scenes/" + name + ".json";
    json sceneJson;
    sceneJson["name"] = name;
    sceneJson["entities"] = SerializeEntities(entities);

    std::ofstream out(file);
    out << sceneJson.dump(4);
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
        GetChildLights(entity->children, lightsOut);
        auto* light = entity->GetComponent<LightComponent>();
        if (!light) continue;

        LightData data;
        data.type = static_cast<int>(light->type);
        data.color = light->color * light->intensity;
        data.position = entity->GetWorldPosition();
        data.direction = entity->transform.GetForwardDirection();
        data.range = light->range;
        data.constant = light->constant;
        data.linear = light->linear;
        data.quadratic = light->quadratic;
        data.spotAngle = light->spotAngle;
        data.castShadows = light->castShadows;

        lightsOut.push_back(data);
        if (light->type == LightType::Directional && light->isMainDirectional)
            mainDirectionalLight = data;
    }
}

void Scene::GetChildLights(std::vector<Entity*>& children, std::vector<LightData>& lightsOut) {
    for (Entity* entity : children) {
        GetChildLights(entity->children, lightsOut);
        auto* light = entity->GetComponent<LightComponent>();
        if (!light) continue;

        LightData data;
        data.type = static_cast<int>(light->type);
        data.color = light->color * light->intensity;
        data.position = entity->GetWorldPosition();
        data.direction = entity->transform.GetForwardDirection();
        data.range = light->range;
        data.constant = light->constant;
        data.linear = light->linear;
        data.quadratic = light->quadratic;
        data.spotAngle = light->spotAngle;
        data.castShadows = light->castShadows;

        lightsOut.push_back(data);
    }
}