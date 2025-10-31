#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 WolrdNormal;
out vec3 WolrdPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    WolrdNormal = mat3(transpose(inverse(model))) * aNormal;
    WolrdPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(WolrdPos, 1.0);
}