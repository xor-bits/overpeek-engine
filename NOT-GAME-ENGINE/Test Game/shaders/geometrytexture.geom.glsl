#version 400 core
layout(points) in;
layout(line_strip, max_vertices = 2) out;

in vec4 _shader_color[1];
flat in int _shader_id[1];

out vec4 shader_color;
out vec2 shader_uv;
flat out int shader_id;

void main() {

	shader_uv = vec2(0.0, 0.0);
	shader_id = _shader_id[0];
	shader_color = _shader_color[0];
	gl_Position = gl_in[0].gl_Position - vec4(0.5, 0.0, 0.0, 0.0);
	EmitVertex();


	shader_uv = vec2(1.0, 0.0);
	shader_id = _shader_id[0];
	shader_color = _shader_color[0];
	gl_Position = gl_in[0].gl_Position + vec4(0.5, 0.0, 0.0, 0.0);
	EmitVertex();
	
	
	//shader_uv = vec2(0.0, 1.0);
	//shader_id = _shader_id[0];
	//shader_color = _shader_color[0];
	//gl_Position = gl_in[0].gl_Position + vec4(0.0, 0.1, 0.0, 0.0);
	//EmitVertex();

	EndPrimitive();
}