#version 400 core

in vec2 uv;
flat in vec2 texture_id;

out vec4 color;

uniform sampler2D tex;

void main()
{
	color = texture(tex, (uv / 16.0) + texture_id / 16.0);
}