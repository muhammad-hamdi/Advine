#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;

// Simple directional light
uniform vec3 lightDir = normalize(vec3(0, -1.0, 0));
uniform vec3 lightColor = vec3(1.0);
uniform vec3 objectColor = vec3(1.0);

void main() {
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, -lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 textureColor = texture(texture_diffuse1, TexCoords).rgb;
    vec3 result = (diffuse + 0.1) * textureColor; // adding ambient 0.1

    FragColor = vec4(vec3(diffuse + 0.2), 1.0);
}
