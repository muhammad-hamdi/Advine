#ifndef SCENE_H
#define SCENE_H

#include "core/Camera.h"
#include "core/GameObject.h"
#include "core/Entity.h"

#include <map>
#include <vector>
#include <memory>

class Scene {
public:
    Scene();
    ~Scene();

    void AddGameObject(GameObject* obj);
    void RemoveGameObject(GameObject* obj);

    void AddCamera(const std::string& name, Camera* camera);
    void SetActiveCamera(const std::string& name);
    void SetActiveCamera(Entity *entity);
    Camera* GetActiveCamera() const;
    Entity* GetActiveCameraEntity() const;

    void Render(const Camera& camera);
    void Update(float deltaTime);

    template<typename T, typename Predicate>
    Entity* Scene::FindEntityWithComponent(Predicate predicate) {
        for (Entity* entity : entities) {
            T* comp = entity->GetComponent<T>();
            if (comp && predicate(comp)) {
                return entity;
            }
        }
        return nullptr;
    }

    // Optional: A way to get the list of all game objects (if needed for other systems)
    const std::vector<GameObject*>& GetGameObjects() const;
    const std::vector<Entity*>& GetEntities() const;

private:
    std::vector<GameObject*> gameObjects;
    std::vector<Entity*> entities;
    Entity* activeCameraEntity = nullptr;
    std::map<std::string, Camera*> cameras;  // Store cameras by name
    Camera* activeCamera = nullptr;          // The active camera
};

#endif // SCENE_H
