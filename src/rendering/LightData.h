#pragma once

#include<glm/glm.hpp>

struct LightData {
    int type;             // 0 = Directional, 1 = Point, 2 = Spot
    glm::vec3 position;        // For point and spot lights
    glm::vec3 color;
    glm::vec3 direction;       // For directional and spot lights

    float range;          // Attenuation range

    float spotAngle;      // In radians for spot lights

    int castShadows;
    int pad1, pad2;       // Padding for std140
};