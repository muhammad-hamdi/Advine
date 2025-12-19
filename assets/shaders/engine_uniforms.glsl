uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

uniform vec3 u_CameraPos;

struct Light {
    int type;             // 0 = Directional, 1 = Point, 2 = Spot
    vec3 color;
    vec3 position;        // For point and spot lights
    float range;          // Attenuation range
    float constant;
    float linear;
    float quadratic;
    vec3 direction;       // For directional and spot lights
    float spotAngle;      // In radians for spot lights
    int castShadows;
};

#define MAX_LIGHTS 16
uniform Light u_Lights[MAX_LIGHTS];
uniform int u_LightCount;

uniform mat4 u_LightSpaceMatrix;

uniform sampler2D u_ShadowMap;
uniform samplerCube u_EnvironmentMap;