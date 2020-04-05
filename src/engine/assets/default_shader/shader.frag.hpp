const std::string shader_frag_gl = R"shader(
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
)shader";

const std::string shader_frag_vk = R"shader(
#version 450
#extension GL_ARB_separate_shader_objects : enable
		
layout(location = 0) in vec2 shader_uv;
layout(location = 1) in vec4 shader_color;

layout(location = 0) out vec4 color;

void main()
{
	color = shader_color;
}
)shader";