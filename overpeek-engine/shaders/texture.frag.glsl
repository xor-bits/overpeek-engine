#version 400 core

in vec2 uv;
flat in ivec4 texture_off;

out vec4 color;

uniform sampler2D tex;

void main()
{
	color = texture(tex, vec2((uv.x / 16.0 * texture_off.x), (uv.y / 16.0 * texture_off.y)) + texture_off.zw / 16.0);
}