#version 330 core

in vec3 worldNormal;
in vec2 texCoord;
in vec3 worldPos;
out vec4 fragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float ambientStrength;
uniform float specularStrength;
uniform float specularShininess;
uniform vec3 viewPos;

void main()
{
    vec3 ambient = ambientStrength * lightColor;
    vec3 lightDir = normalize(lightPos - worldPos);
    vec3 pixelNormal = normalize(worldNormal);
    vec3 diffuse = max(dot(pixelNormal, lightDir), 0.0) * lightColor;

    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, pixelNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularShininess);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    fragColor = vec4(result, 1.0);
}
