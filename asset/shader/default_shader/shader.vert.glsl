#version 140
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec2 texture_uv;
layout(location = 2) in vec4 vertex_color;

out vec2 shader_uv_frag;
out vec4 shader_color_frag;

uniform mat4 mvp_matrix = mat4(1.0);

void main()
{
	gl_Position = mvp_matrix * vec4(vertex_pos, 1.0);

	shader_uv_frag = texture_uv;
	shader_color_frag = vertex_color;
}