#version 330 core

in vec4 vertexColor;
in vec2 texCoord;
out vec4 fragColor;
uniform sampler2D texSampler1;
uniform sampler2D texSampler2;

void main()
{
    if (texCoord.x < 0.5)
    {
        fragColor = texture(texSampler1, texCoord);
    }
    else
    {
        fragColor = texture(texSampler2, texCoord);
    }
}
