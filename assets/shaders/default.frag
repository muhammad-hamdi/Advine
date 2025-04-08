#version 450 core

#define MAX_LIGHTS 16

struct Light {
    int type;             // 0 = Directional, 1 = Point, 2 = Spot
    vec3 color;
    vec3 position;        // For point and spot lights
    float range;          // Attenuation range
    vec3 direction;       // For directional and spot lights
    float spotAngle;      // In radians for spot lights
    float intensity;

    int castShadows;
    int pad1, pad2;       // Padding for std140
};

layout(std140) uniform Lights {
    Light u_Lights[MAX_LIGHTS];
    int u_LightCount;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_SpecularTexture;

uniform vec3 u_Light;
uniform vec3 u_CameraPos;
uniform float specularStrength = 0.8;
uniform vec3 lightColor = vec3(1.0);
uniform vec3 objectColor = vec3(0.8); // used if no texture

out vec4 FragColor;

vec3 CalculateLight(vec3 finalColor, vec3 diffuseColor, vec3 specularColor) {
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(u_CameraPos - FragPos);
    for (int i = 0; i < u_LightCount; ++i) {
        Light light = u_Lights[i];
        
        // Directional Light
        if (light.type == 0) {
            // Directional light doesn't have a position, only uses direction
            vec3 lightDir = normalize(light.direction);
            vec3 diffuse = max(dot(normal, -lightDir), 0.0) * light.color * diffuseColor;
            vec3 specular = pow(max(dot(reflect(-lightDir, normal), viewDir), 0.0), 32.0) * specularColor;

            finalColor += diffuse + specular;

        } 
        else if (light.type == 1) {        // Point Light
            // Point light has a position, calculate distance-based attenuation
            vec3 lightDir = normalize(light.position - FragPos);
            float distance = length(light.position - FragPos);
            float attenuation = 1.0 / (distance * distance);  // Simple attenuation
            attenuation *= (distance <= light.range) ? 1.0 : 0.0;  // Apply range to attenuation

            vec3 diffuse = max(dot(normal, -lightDir), 0.0) * light.color * diffuseColor * attenuation;
            vec3 specular = pow(max(dot(reflect(-lightDir, normal), viewDir), 0.0), 32.0) * specularColor * attenuation;

            finalColor += diffuse + specular;

        } 
        else if (light.type == 2) {        // Spot Light
            // Spot light has both position and direction, with attenuation
            vec3 lightDir = normalize(light.position - FragPos);
            float distance = length(light.position - FragPos);
            float attenuation = 1.0 / (distance * distance);  // Simple attenuation
            attenuation *= (distance <= light.range) ? 1.0 : 0.0;  // Apply range to attenuation

            // Calculate spotlight effect using spotAngle
            float theta = dot(lightDir, normalize(-light.direction)); // Dot between light direction and fragment direction
            float spotFactor = smoothstep(cos(light.spotAngle), 1.0, theta); // Apply spot angle to get soft edge of cone

            vec3 diffuse = max(dot(normal, -lightDir), 0.0) * light.color * diffuseColor * attenuation * spotFactor;
            vec3 specular = pow(max(dot(reflect(-lightDir, normal), viewDir), 0.0), 32.0) * specularColor * attenuation * spotFactor;

            finalColor += diffuse + specular;
        }
    }

    return finalColor;
}

void main()
{
    vec3 diffuseColor = texture(u_DiffuseTexture, TexCoord).rgb;
    if (length(diffuseColor) <= 0.01) diffuseColor = objectColor;
    vec3 specularColor = texture(u_SpecularTexture, TexCoord).rgb;
    if (length(specularColor) <= 0.01) specularColor = vec3(1.0);
    
    // Ambient
    vec3 ambientLightColor = vec3(0.1);
    vec3 ambient = ambientLightColor * diffuseColor;
    vec3 finalColor = ambient;

#if 0

    vec3 norm = normalize(Normal);
    // vec3 lightDir = normalize(lightPos - FragPos);
    vec3 lightDir = normalize(vec3(5, -1, -1));

    // Diffuse
    float diff = max(dot(norm, -lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(u_CameraPos - FragPos);

    // Specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor * specularColor;

    // Texture or fallback color
    vec3 texColor = texture(u_DiffuseTexture, TexCoord).rgb;
    if (length(texColor) <= 0.01) texColor = objectColor;

    vec3 result = (ambient + diffuse + specular) * diffuseColor;
    FragColor = vec4(result, 1.0);
#else
    finalColor = CalculateLight(finalColor, diffuseColor, specularColor);
    FragColor = vec4(finalColor, 1.0);
#endif
}
