#version 450 core
#include "engine_uniforms.glsl"

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

struct Material {
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
};

uniform Material u_Material;
uniform bool u_HasAlbedoMap;
uniform sampler2D u_AlbedoMap;
uniform bool u_HasNormalMap;
uniform sampler2D u_NormalMap;
uniform bool u_HasMetallicMap;
uniform sampler2D u_MetallicMap;
uniform bool u_HasRoughnessMap;
uniform sampler2D u_RoughnessMap;
uniform bool u_HasAoMap;
uniform sampler2D u_AoMap;

uniform int u_IsLit;

out vec4 FragColor;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

void main()
{
    vec3 albedo = u_HasAlbedoMap ? texture(u_AlbedoMap, fs_in.TexCoords).rgb : u_Material.albedo;
    float metallic = u_HasMetallicMap ? texture(u_MetallicMap, fs_in.TexCoords).r : u_Material.metallic;
    float roughness = u_HasRoughnessMap ? texture(u_RoughnessMap, fs_in.TexCoords).r : u_Material.roughness;
    float ao = u_HasAoMap ? texture(u_AoMap, fs_in.TexCoords).r : u_Material.ao;

    vec3 N = normalize(fs_in.Normal);
    vec3 V = normalize(u_CameraPos - fs_in.FragPos);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    for(int i = 0; i < u_LightCount; ++i)
    {
        vec3 L;
        float attenuation = 1.0;
        vec3 radiance = u_Lights[i].color;

        if (u_Lights[i].type == 0) { // Directional Light
            L = normalize(-u_Lights[i].direction);
        } else { // Point or Spot Light
            vec3 lightDir = u_Lights[i].position - fs_in.FragPos;
            float distance = length(lightDir);
            L = normalize(lightDir);
            attenuation = 1.0 / (u_Lights[i].constant + u_Lights[i].linear * distance + u_Lights[i].quadratic * (distance * distance));
            
            if (u_Lights[i].type == 2) { // Spot Light
                float theta = dot(L, normalize(-u_Lights[i].direction));
                float epsilon = u_Lights[i].spotAngle - 0.01; // Inner cone, hardcoded for now
                float intensity = clamp((theta - u_Lights[i].spotAngle) / epsilon, 0.0, 1.0);
                attenuation *= intensity;
            }
        }
        radiance *= attenuation;

        vec3 H = normalize(V + L);

        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + (NDF * G * F) / (4.0 * max(dot(N, V), 0.0) * NdotL + 0.001)) * radiance * NdotL;
    }

    vec3 ambient = vec3(0.03) * albedo * ao + texture(u_EnvironmentMap, N).rgb;
    vec3 color = ambient;
    if (u_IsLit == 1) {
        color += Lo;
    }

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}