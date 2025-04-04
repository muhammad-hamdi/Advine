#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include <map>
#include "game/GameObject.h"
#include "Camera.h"

class Scene {
public:
    Scene();
    ~Scene();

    void AddGameObject(GameObject* obj);
    void RemoveGameObject(GameObject* obj);

    void AddCamera(const std::string& name, Camera* camera);
    void SetActiveCamera(const std::string& name);
    Camera* GetActiveCamera() const;

    void Render(const Camera& camera);
    void Update(float deltaTime);

    // Optional: A way to get the list of all game objects (if needed for other systems)
    const std::vector<GameObject*>& GetGameObjects() const;

private:
    std::vector<GameObject*> gameObjects;
    std::map<std::string, Camera*> cameras;  // Store cameras by name
    Camera* activeCamera = nullptr;          // The active camera
};

#endif // SCENE_H
