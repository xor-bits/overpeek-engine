#version 400 core
layout (location = 0) in vec2 vertex_pos;
layout (location = 1) in vec2 vertex_uv;
layout (location = 2) in vec4 vertex_texture_off;

out vec2 uv;
flat out ivec4 texture_off;

uniform mat4 pr_matrix;
uniform mat4 ml_matrix = mat4(1.0);

uniform ivec4 overwrite_off = ivec4(0);

void main()
{
	gl_Position = pr_matrix * ml_matrix * vec4(vertex_pos.x, vertex_pos.y, 0.0f, 1.0f);
	uv = vertex_uv;
	
	if (overwrite_off != vec4(0)) {
		texture_off = overwrite_off;
	} else {
		texture_off = ivec4(vertex_texture_off);
	}
}