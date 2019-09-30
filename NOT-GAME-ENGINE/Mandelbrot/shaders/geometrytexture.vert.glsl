#version 400 core
layout(location = 0) in vec2 vertex_pos;
layout(location = 1) in vec2 vertex_size;
layout(location = 2) in float texture_id;
layout(location = 3) in vec4 vertex_color;

out vec2 _shader_size;
out vec4 _shader_color;
flat out int _shader_id;

void main()
{
	gl_Position = vec4(vertex_pos.x, vertex_pos.y, 0.0f, 1.0f);
	_shader_id = int(floor(texture_id));
	_shader_color = vertex_color;
	_shader_size = vertex_size;
}