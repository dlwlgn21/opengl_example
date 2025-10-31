#version 330 core

in vec4 vertexColor;
in vec2 texCoord;
out vec4 fragColor;
uniform sampler2D texSampler;

void main()
{
  	vec4 pixel = texture(texSampler, texCoord); 
	if (pixel.a < 0.01)
    {
        discard;
    }
    fragColor = pixel;
}