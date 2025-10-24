#version 330 core

in vec3 WorldNormal;
in vec2 TexCoord;
in vec3 WorldPos;
out vec4 FragColor;

uniform vec3 viewWorldPos;
struct Light
{
    vec3 Pos;
    vec3 Dir;
    float Cutoff;
    vec3 Att;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};
uniform Light light;

struct Material
{
    sampler2D Diffuse;
    sampler2D Specular;
    float Shininess;
};
uniform Material material;

void main()
{
    // vec3 texColor = texture2D(material.Diffuse, TexCoord).xyz;
    // vec3 ambient = texColor * light.Ambient;

    // vec3 lightDir = normalize(-light.Direction);
    // vec3 pixelNorm = normalize(WorldNormal);
 


    // float dist = length(light.Pos - WorldPos);
    // vec3 distPoly = vec3(1.0, dist, dist * dist); // 1.0f, d, d*d
    // float att = 1.0 / dot(distPoly, light.Att);
    // vec3 lightDir = normalize(light.Pos - WorldPos) / dist;
    // vec3 pixelNorm = normalize(WorldNormal);
    // float diff = max(dot(pixelNorm, lightDir), 0.0);
    // vec3 diffuse = diff * texColor * light.Diffuse;

    
    // vec3 specColor = texture2D(material.Specular, TexCoord).xyz;
    // vec3 viewDir = normalize(viewWorldPos - WorldPos);
    // vec3 reflectDir = reflect(-lightDir, pixelNorm);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
    // vec3 specular = spec * specColor * light.Specular;

    // vec3 result = (ambient + diffuse + specular) * att;
    // FragColor = vec4(result, 1.0);

    float dist = length(light.Pos - WorldPos);
    vec3 distPoly = vec3(1.0, dist, dist * dist); // 1.0f, d, d*d
    float att = 1.0 / dot(distPoly, light.Att);

    vec3 texColor = texture2D(material.Diffuse, TexCoord).xyz;
    vec3 ambient = texColor * light.Ambient;
    vec3 lightDir = normalize(light.Pos - WorldPos) / dist;

    float cosTheta = dot(lightDir, normalize(-light.Dir));
    vec3 result = ambient;

    if (light.Cutoff < cosTheta)
    {
        vec3 pixelNorm = normalize(WorldNormal);
        float diff = max(dot(pixelNorm, lightDir), 0.0);
        vec3 diffuse = diff * texColor * light.Diffuse;

        vec3 specColor = texture2D(material.Specular, TexCoord).xyz;
        vec3 viewDir = normalize(viewWorldPos - WorldPos);
        vec3 reflectDir = reflect(-lightDir, pixelNorm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
        vec3 specular = spec * specColor * light.Specular;
        result += diffuse + specular;
    }

    result *= att;
    FragColor = vec4(result, 1.0);
}
