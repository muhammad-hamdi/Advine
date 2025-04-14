#pragma once
#include "json.hpp"

class Entity;

class Component {
public:
    virtual ~Component() = default;
    Entity* owner = nullptr;

    virtual void Update(float deltaTime) {};
    virtual bool IsOverridden() const { return false; }
    virtual nlohmann::json Serialize() const { return {}; };
};
