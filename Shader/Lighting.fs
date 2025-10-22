#version 330 core

in vec3 WorldNormal;
in vec2 TexCoord;
in vec3 WorldPos;
out vec4 FragColor;

uniform vec3 viewWorldPos;
struct Light
{
    vec3 WorldPos;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};
uniform Light light;

struct Material
{
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Shininess;
};
uniform Material material;

void main()
{
    vec3 ambient = material.Ambient * light.Ambient;
    vec3 lightDir = normalize(light.WorldPos - WorldPos);
    vec3 pixelNorm = normalize(WorldNormal);
    float diff = max(dot(pixelNorm, lightDir), 0.0);
    vec3 diffuse = diff * material.Diffuse * light.Diffuse;
 
    vec3 viewDir = normalize(viewWorldPos - WorldPos);
    vec3 reflectDir = reflect(-lightDir, pixelNorm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
    vec3 specular = spec * material.Specular * light.Specular;
 
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
