#version 400 core

in vec4 shader_color;
in vec2 shader_uv;
flat in int shader_id;

layout(location = 0) out vec4 color;

uniform sampler2DArray tex;

void main()
{
	vec4 textureColor = texture(tex, vec3(shader_uv, shader_id));
	textureColor.x *= shader_color.x;
	textureColor.y *= shader_color.y;
	textureColor.z *= shader_color.z;
	textureColor.a *= shader_color.a;

	color = textureColor;
}