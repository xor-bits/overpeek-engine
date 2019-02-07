#version 400 core
layout(location = 0) in vec2 vertex_pos;
layout(location = 1) in float texture_id;
layout(location = 2) in vec4 vertex_color;

out vec4 _shader_color;
flat out int _shader_id;

uniform mat4 pr_matrix;
uniform mat4 ml_matrix = mat4(1.0);

void main()
{
	gl_Position = pr_matrix * ml_matrix * vec4(vertex_pos.x, vertex_pos.y, 0.0f, 1.0f);
	_shader_id = int(floor(texture_id));
	_shader_color = vertex_color;
}