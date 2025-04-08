#version 450 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture_diffuse;

uniform vec3 u_Light;
uniform vec3 u_CameraPos;
uniform float specularStrength = 0.8;
uniform vec3 lightColor = vec3(1.0);
uniform vec3 objectColor = vec3(0.8); // used if no texture

out vec4 FragColor;

void main()
{
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    // vec3 lightDir = normalize(lightPos - FragPos);
    vec3 lightDir = normalize(vec3(5, 1, 1));

    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(u_CameraPos - FragPos);

    // Specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Texture or fallback color
    vec3 texColor = texture(texture_diffuse, TexCoord).rgb;
    if (length(texColor) <= 0.01) texColor = objectColor;

    vec3 result = (ambient + diffuse + specular) * texColor;
    FragColor = vec4(result, 1.0);
}
