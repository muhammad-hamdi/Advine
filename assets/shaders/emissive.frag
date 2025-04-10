#version 450 core

#define MAX_LIGHTS 16

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D u_DiffuseTexture1;
uniform sampler2D u_DiffuseTexture2;
uniform sampler2D u_DiffuseTexture3;
uniform sampler2D u_SpecularTexture1;
uniform sampler2D u_SpecularTexture2;

uniform vec3 u_CameraPos;
uniform vec3 u_ObjectColor = vec3(0.8); // used if no texture
uniform float u_SpecularStrength = 0.8;

out vec4 FragColor;

void main()
{
    vec3 diffuseColor = texture(u_DiffuseTexture1, TexCoord).rgb;    

    FragColor = vec4(diffuseColor, 1.0);
}
