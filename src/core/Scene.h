#pragma once

#include "assets/Model.h"

#include "core/Camera.h"
#include "core/Entity.h"
#include "rendering/LightData.h"

#include "nlohmann/json.hpp"

#include <map>
#include <vector>
#include <memory>

namespace Engine {
    using json = nlohmann::json;
    struct LightData;

    class Scene {
    public:
        Scene();
        Scene(std::string path);
        ~Scene();

        std::vector<Entity*> LoadEntitesFromJson(json jsonEntities, Model& loader, Entity* parent);

        void LoadFromFile();

        json SerializeEntities(const std::vector<Entity*>& entities);

        void SaveToFile();

        void AddCamera(const std::string& name, Camera* camera);
        void SetActiveCamera(const std::string& name);
        void SetActiveCamera(Entity* entity);
        Camera* GetActiveCamera() const;
        Entity* GetActiveCameraEntity() const;
        Entity* GetDirectionalLightEntity() const;
        Entity* CreateEntity(const std::string& name);
        void AddEntity(Entity* entity);
        
        inline Mesh* GetSkyboxMesh() const { return mSkyboxMesh; };
        
        void Update(float deltaTime);

        template<typename T, typename Predicate>
        Entity* FindEntityWithComponent(Predicate predicate) {
            for (Entity* entity : entities) {
                T* comp = entity->GetComponent<T>();
                if (comp && predicate(comp)) {
                    return entity;
                }
            }
            return nullptr;
        }

        // Optional: A way to get the list of all game objects (if needed for other systems)
        const std::vector<Entity*>& GetEntities() const;

        void GatherLights(std::vector<LightData>& lightsOut);

        void GetChildLights(std::vector<Entity*>& children, std::vector<LightData>& lightsOut);

    private:
        std::vector<Entity*> entities;
        Entity* activeCameraEntity = nullptr;
        LightData mainDirectionalLight;
        std::map<std::string, Camera*> cameras;  // Store cameras by name
        Camera* activeCamera = nullptr;          // The active camera
        Mesh* mSkyboxMesh;

        std::string filepath;
        std::string name;
    };
}