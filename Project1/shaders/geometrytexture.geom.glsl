#version 400 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 _shader_uv[3];
in vec4 _shader_color[3];
flat in int _shader_id[3];

out vec4 shader_color;
out vec2 shader_uv;
flat out int shader_id;

void main() {

	shader_uv = _shader_uv[0];
	shader_id = _shader_id[0];
	shader_color = _shader_color[0];
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();


	shader_uv = _shader_uv[1];
	shader_id = _shader_id[1];
	shader_color = _shader_color[1];
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();
	
	
	shader_uv = _shader_uv[2];
	shader_id = _shader_id[2];
	shader_color = _shader_color[2];
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();

	EndPrimitive();
}