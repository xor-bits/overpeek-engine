#version 330
		
in vec2 shader_uv;
in vec4 shader_color;

layout(location = 0) out vec4 color;

uniform sampler2D tex;
uniform int usetex = 1;

void main()
{
	if (usetex != 0) color = texture(tex, shader_uv) * shader_color;
	else color = shader_color;
}