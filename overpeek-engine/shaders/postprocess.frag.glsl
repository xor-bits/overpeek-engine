#version 400 core

out vec4 color;

in vec2 UV;

uniform sampler2D renderedTexture;

void main()
{
	color = texture(renderedTexture, UV);
}