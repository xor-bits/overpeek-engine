#version 330 core

layout(location = 0) out vec4 color;

in vec2 shader_uv;
in vec4 shader_color;
flat in int shader_id;

uniform sampler2DArray tex;
uniform bool unif_text;
uniform bool unif_blending;

void main()
{	
	color = vec4(1.0, 0.5, 0.0, 1.0);
}