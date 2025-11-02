#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out VS_OUT {
  vec3 FragWorldPos;
  vec3 WorldNormal;
  vec2 TexCoord;
  vec4 FragPosLight;
} vs_out;

uniform mat4 transform;
uniform mat4 modelTransform;
uniform mat4 lightTransform;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0);
    vs_out.FragWorldPos = vec3(modelTransform * vec4(aPos, 1.0));
    vs_out.WorldNormal = transpose(inverse(mat3(modelTransform))) * aNormal;
    vs_out.TexCoord = aTexCoord;
    vs_out.FragPosLight = lightTransform * vec4(vs_out.FragWorldPos, 1.0);
}