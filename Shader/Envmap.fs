#version 330 core

out vec4 fragColor;

in vec3 WolrdNormal;
in vec3 WolrdPos;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
    vec3 I = normalize(WolrdPos - cameraPos);
    vec3 R = reflect(I, normalize(WolrdNormal));
    fragColor = vec4(texture(skybox, R).rgb, 1.0);
}