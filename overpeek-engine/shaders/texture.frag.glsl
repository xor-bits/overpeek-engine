#version 400 core

in vec2 uv;
flat in vec2 texture_off;

out vec4 color;

uniform sampler2D tex;

void main()
{
	color = texture(tex, (uv / 16.0) + (texture_off / 16.0));
}