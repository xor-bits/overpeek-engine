constexpr char shader_frag_gl[] = R"glsl(
#version 140
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) out vec4 color;
		
in vec2 shader_uv_frag;
in vec4 shader_color_frag;

uniform sampler2D u_tex;
uniform int u_usetex = 1;
uniform vec4 u_color = vec4(1.0);

void main()
{
#if 1 // BRANCHLESS
	color = int(u_usetex==1) * texture(u_tex, shader_uv_frag) * shader_color_frag * u_color
		+ int(u_usetex==0) * shader_color_frag * u_color;
#else // BRANCHED
	if (u_usetex != 0) color = texture(u_tex, shader_uv_frag) * shader_color_frag * u_color;
	else color = shader_color_frag * u_color;
#endif
}
)glsl";

constexpr char shader_frag_vk[] = R"glsl(
#version 140
#extension GL_ARB_separate_shader_objects : enable
		
layout(location = 0) in vec2 shader_uv;
layout(location = 1) in vec4 shader_color;

layout(location = 0) out vec4 color;

void main()
{
	color = shader_color;
}
)glsl";