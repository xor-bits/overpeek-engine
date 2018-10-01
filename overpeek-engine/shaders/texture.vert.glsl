#version 400 core
layout (location = 0) in vec2 vertex_pos;
layout (location = 1) in vec2 vertex_uv;
layout (location = 2) in int vertex_texture_off;

out vec2 uv;
flat out vec2 texture_off;

uniform mat4 pr_matrix;
uniform mat4 ml_matrix = mat4(1.0);

uniform int overwrite_off = 0;

void main()
{
	gl_Position = pr_matrix * ml_matrix * vec4(vertex_pos.x, vertex_pos.y, 0.0f, 1.0f);
	uv = vertex_uv;
	
	if (overwrite_off != 0) {
		texture_off.x = int(overwrite_off) % 16;
		texture_off.y = (overwrite_off - texture_off.x) / 16;
	} else {
		texture_off.x = int(vertex_texture_off) % 16;
		texture_off.y = (vertex_texture_off - texture_off.x) / 16;
	}
}