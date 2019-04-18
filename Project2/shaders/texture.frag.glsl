#version 400 core

in vec2 shader_uv;
in vec4 shader_color;
flat in int shader_id;

layout(location = 0) out vec4 color;

uniform sampler2DArray tex;

void main()
{
	color = texture(tex, vec3(shader_uv, shader_id)) * shader_color;
}