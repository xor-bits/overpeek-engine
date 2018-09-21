#version 400 core
layout (location = 0) in vec2 vertex_pos;
layout (location = 1) in vec2 vertex_uv;

out vec2 uv;

uniform mat4 pr_matrix;
uniform mat4 ml_matrix = mat4(1.0);

void main()
{
	gl_Position = pr_matrix * ml_matrix * vec4(vertex_pos, 0.0f, 1.0f);
	uv = vertex_uv;
}