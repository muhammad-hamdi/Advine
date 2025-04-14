#pragma once

#include "core/Component.h"
#include "glm/glm.hpp"

enum class LightType {
    Directional,
    Point,
    Spot
};

struct LightComponent : public Component {
    LightType type = LightType::Point;

    glm::vec3 color = glm::vec3(1.0f);
    float intensity = 1.0f;

    float range = 10.0f; // Only used for Point & Spot lights
    float constant = 1.0f;
    float linear = 0.09;
    float quadratic = 0.032;

    float spotAngle = glm::radians(30.0f); // Only used for Spot lights

    bool castShadows = false;
    bool isMainDirectional = false; // Renderer can pick this as the sun

    LightComponent(LightType type)
        : type(type), color(1.0f), intensity(1.0f), range(10.0f), spotAngle(glm::radians(30.0f)) {}

    bool IsOverridden() const {
        return true;
    }

    json Serialize() const {
        json j;
        j["name"] = "light";
        j["type"] = static_cast<int>(type);
    
        j["color"] = { color.r, color.g, color.b };
        j["intensity"] = intensity;

        j["spotAngle"] = spotAngle;

        j["range"] = range;

        if (type != LightType::Directional) {
            j["constant"]  = constant;
            j["linear"]    = linear;
            j["quadratic"] = quadratic;
        }
    
        return j;
    }
};
