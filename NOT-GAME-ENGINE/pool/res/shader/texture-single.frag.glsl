#version 400 core

in vec2 shader_uv;
in vec4 shader_color;

layout(location = 0) out vec4 color;

uniform sampler2D tex;

void main()
{
	vec4 textureColor = texture(tex, shader_uv);
	color = textureColor;
}