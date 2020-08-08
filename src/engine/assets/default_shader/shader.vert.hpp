constexpr char shader_vert_gl[] = R"shader(
#version 140
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec2 texture_uv;
layout(location = 2) in vec4 vertex_color;

out vec2 shader_uv_frag;
out vec4 shader_color_frag;

uniform mat4 pr_matrix = mat4(1.0);
uniform mat4 vw_matrix = mat4(1.0);
uniform mat4 ml_matrix = mat4(1.0);

void main()
{
	mat4 mvp = pr_matrix * vw_matrix * ml_matrix;
	gl_Position = mvp * vec4(vertex_pos, 1.0);

	shader_uv_frag = texture_uv;
	shader_color_frag = vertex_color;
}
)shader";

constexpr char shader_vert_vk[] = R"shader(
#version 140
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec2 texture_uv;
layout(location = 2) in vec4 vertex_color;

layout(location = 0) out vec2 shader_uv;
layout(location = 1) out vec4 shader_color;

mat4 pr_matrix = mat4(1.0);
mat4 ml_matrix = mat4(1.0);
mat4 vw_matrix = mat4(1.0);

void main()
{
	mat4 mvp = pr_matrix * vw_matrix * ml_matrix;
	gl_Position = mvp * vec4(vertex_pos, 1.0);

	shader_uv = texture_uv;
	shader_color = vertex_color;
}
)shader";