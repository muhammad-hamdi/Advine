#pragma once
#include "core/Transform.h"
#include "core/Component.h"

#include "json.hpp"

#include <vector>
#include <string>
#include <memory>

using json = nlohmann::json;

class Entity {
public:
    std::string name;
    std::string modelPath;
    int source = 0; // 0: model, 1: editor
    bool isModelRoot = false;
    Transform transform; // Local transform
    Entity* parent = nullptr; // Optional parent
    std::vector<Entity*> children;
    std::vector<std::unique_ptr<Component>> components;

    template<typename T, typename... Args>
    T* AddComponent(Args&&... args) {
        static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
        T* component = new T(std::forward<Args>(args)...);
        component->owner = this;
        components.emplace_back(component);
        return component;
    }

    template<typename T>
    T* GetComponent() {
        for (const auto& comp : components) {
            if (auto casted = dynamic_cast<T*>(comp.get())) {
                return casted;
            }
        }
        return nullptr;
    }

    bool Entity::HasTransformOverride() const {
        return transform.positionOverridden || transform.rotationOverridden || transform.scaleOverridden;
    }

    glm::mat4 GetWorldMatrix() const;

    glm::vec3 GetWorldPosition() const;

    void Update(float deltaTime);
    bool HasOverrides() const;
    void GatherOverrides(std::vector<json> &overridesOut, const std::vector<int> &path) const;
};
