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

    glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);// Only used for Directional & Spot lights

    float range = 10.0f; // Only used for Point & Spot lights
    float constant = 1.0f;
    float linear = 0.22;
    float quadratic = 0.20;

    float spotAngle = glm::radians(30.0f); // Only used for Spot lights

    bool castShadows = false;
    bool isMainDirectional = false; // Renderer can pick this as the sun

    LightComponent(LightType type)
        : type(type), color(1.0f), intensity(1.0f), range(10.0f), spotAngle(glm::radians(30.0f)) {}
};
