#version 450 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture_diffuse;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor = vec3(1.0);
uniform vec3 objectColor = vec3(0.5); // used if no texture

out vec4 FragColor;

void main()
{
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Texture or fallback color
    vec3 texColor = texture(texture_diffuse, TexCoord).rgb;
    if (length(texColor) <= 0.01) texColor = objectColor;

    vec3 result = (ambient + diffuse) * texColor;
    FragColor = vec4(result, 1.0);
}
