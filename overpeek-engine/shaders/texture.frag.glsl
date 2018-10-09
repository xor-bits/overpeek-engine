#version 400 core

in vec2 uv;
flat in int id;

out vec4 color;

uniform sampler2DArray tex;

void main()
{
	color = texture(tex, vec3(uv, id));
}