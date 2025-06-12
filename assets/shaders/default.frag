#version 450 core
#include "engine_uniforms.glsl"

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform int u_IsLit;

uniform sampler2D u_DiffuseTexture1;
uniform sampler2D u_DiffuseTexture2;
uniform sampler2D u_DiffuseTexture3;
uniform sampler2D u_SpecularTexture1;
uniform sampler2D u_SpecularTexture2;

uniform vec3 u_ObjectColor = vec3(0.8); // used if no texture
uniform float u_SpecularStrength = 0.8;

out vec4 FragColor;

vec3 CalculateDirectionalLight(Light light, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularColor) {
    vec3 lightDir = normalize(light.direction);
    vec3 diffuse = max(dot(normal, -lightDir), 0.0) * light.color * diffuseColor;
    vec3 specular = pow(max(dot(reflect(lightDir, normal), viewDir), 0.0), 32.0) * specularColor * light.color * u_SpecularStrength;

    return diffuse + specular;
}

vec3 CalculatePointLight(Light light, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularColor) {
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    float distance = length(light.position - fs_in.FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    attenuation *= (distance <= light.range) ? 1.0 : 0.0;  // Apply range to attenuation

    vec3 diffuse = max(dot(normal, lightDir), 0.0) * light.color * diffuseColor * attenuation;
    vec3 specular = pow(max(dot(reflect(-lightDir, normal), viewDir), 0.0), 32.0) * specularColor * attenuation * light.color * u_SpecularStrength;

    return diffuse + specular;
}

vec3 CalculateSpotLight(Light light, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularColor) {
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    float distance = length(light.position - fs_in.FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    // attenuation *= (distance <= light.range) ? 1.0 : 0.0;  // Apply range to attenuation

    // Calculate spotlight effect using spotAngle
    float theta = dot(lightDir, normalize(-light.direction)); // Dot between light direction and fragment direction
    // float spotFactor = smoothstep(cos(light.spotAngle), 1.0, theta); // Apply spot angle to get soft edge of cone

    // vec3 diffuse = max(dot(normal, lightDir), 0.0) * light.color * diffuseColor * attenuation * spotFactor;
    // vec3 specular = pow(max(dot(reflect(-lightDir, normal), viewDir), 0.0), 32.0) * specularColor * attenuation * spotFactor * light.color * u_SpecularStrength;
    // return diffuse + specular;

    if(theta > cos(light.spotAngle)) {
        vec3 diffuse = max(dot(normal, lightDir), 0.0) * light.color * diffuseColor * attenuation;
        vec3 specular = pow(max(dot(reflect(-lightDir, normal), viewDir), 0.0), 32.0) * specularColor * attenuation * light.color * u_SpecularStrength;
        return diffuse + specular;
    }
}

vec3 CalculateLight(vec3 finalColor, vec3 diffuseColor, vec3 specularColor) {
    vec3 normal = normalize(fs_in.Normal);
    vec3 viewDir = normalize(u_CameraPos - fs_in.FragPos);
    for (int i = 0; i < u_LightCount; ++i) {
        Light light = u_Lights[i];
        if (light.type == 0) {
            finalColor += CalculateDirectionalLight(light, normal, viewDir, diffuseColor, specularColor);
        }
        else if (light.type == 1) {
            finalColor += CalculatePointLight(light, normal, viewDir, diffuseColor, specularColor);
        } 
        else if (light.type == 2) {
            finalColor += CalculateSpotLight(light, normal, viewDir, diffuseColor, specularColor);
        }
    }
    return finalColor;
}


float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(u_ShadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;


    // better bias when we have access to light direction
    // vec3 lightDir;
    // for (int i = 0; i < u_LightCount; ++i) {
    //     if (u_Lights[i].type == 0) {
    //         lightDir = u_Lights[i].direction;
    //         break;
    //     }
    // }
    // Light light = u_Lights[directionalIndex];
    // vec3 normal = normalize(fs_in.Normal);
    // float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    float bias = 0.005;
    // check whether current frag pos is in shadow
    float shadow = currentDepth - bias > closestDepth  ? 0.5 : 0.0;
    if(projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}

void main()
{
    vec3 diffuseColor = texture(u_DiffuseTexture1, fs_in.TexCoords).rgb;
    if (length(diffuseColor) <= 0.01) diffuseColor = u_ObjectColor;

    vec3 specularColor = texture(u_SpecularTexture1, fs_in.TexCoords).rgb;
    if (length(specularColor) <= 0.01) specularColor = vec3(0.2); // TODO: add material (ambient, diffuse, specular) properties as fallback

    vec3 ambientLightColor = vec3(0.1); // TODO: add global ambient color, maybe the clear color? or the directional light color but damped
    vec3 ambient = ambientLightColor * diffuseColor;
    vec3 finalColor = ambient;

    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);

    if(u_IsLit != 0) {
        finalColor = (finalColor + (1.0 - shadow)) * CalculateLight(finalColor, diffuseColor, specularColor);
    } else {
        finalColor = diffuseColor;
    }
    FragColor = vec4(finalColor, 1.0);
}
