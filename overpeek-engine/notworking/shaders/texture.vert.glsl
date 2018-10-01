#version 400 core
layout (location = 0) in vec2 vertex_pos;
layout (location = 1) in vec2 vertex_uv;
layout (location = 2) in float vertex_texture_id;

out vec2 uv;
flat out vec2 texture_id;

uniform int overwrite_id = 0;
uniform mat4 pr_matrix;
uniform mat4 ml_matrix = mat4(1.0);

void main()
{
	gl_Position = pr_matrix * ml_matrix * vec4(vertex_pos.x, vertex_pos.y, 0.0f, 1.0f);
	uv = vertex_uv;

	if (overwrite_id != 0) {
		texture_id.x = (int(overwrite_id) % 16);
		texture_id.y = (overwrite_id - texture_id.x) / 16;
	} else {
		texture_id.x = (int(vertex_texture_id) % 16);
		texture_id.y = (vertex_texture_id - texture_id.x) / 16;
	}
}