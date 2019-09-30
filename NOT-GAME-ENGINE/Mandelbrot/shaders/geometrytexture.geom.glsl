#version 400 core
layout(points) in;
layout(triangle_strip, max_vertices = 6) out;

in vec2 _shader_size[1];
in vec4 _shader_color[1];
flat in int _shader_id[1];

out vec4 shader_color;
out vec2 shader_uv;
flat out int shader_id;

uniform mat4 pr_matrix = mat4(1.0);

void main() {

	float w = _shader_size[0].x;
	float h = _shader_size[0].y;

	//Triangle 0
	shader_uv = vec2(0.0, 0.0);
	shader_id = _shader_id[0];
	shader_color = _shader_color[0];
	gl_Position = pr_matrix * gl_in[0].gl_Position;
	EmitVertex();
    
	shader_uv = vec2(1.0, 0.0);
	shader_id = _shader_id[0];
	shader_color = _shader_color[0];
	gl_Position = pr_matrix * (gl_in[0].gl_Position + vec4(w, 0.0, 0.0, 0.0));
	EmitVertex();
    
	shader_uv = vec2(1.0, 1.0);
	shader_id = _shader_id[0];
	shader_color = _shader_color[0];
	gl_Position = pr_matrix * (gl_in[0].gl_Position + vec4(w, h, 0.0, 0.0));
	EmitVertex();
	EndPrimitive();
    
	//Triangle 1
	shader_uv = vec2(0.0, 0.0);
	shader_id = _shader_id[0];
	shader_color = _shader_color[0];
	gl_Position = pr_matrix * gl_in[0].gl_Position;
	EmitVertex();
    
	shader_uv = vec2(1.0, 1.0);
	shader_id = _shader_id[0];
	shader_color = _shader_color[0];
	gl_Position = pr_matrix * (gl_in[0].gl_Position + vec4(w, h, 0.0, 0.0));
	EmitVertex();
    
	shader_uv = vec2(0.0, 1.0);
	shader_id = _shader_id[0];
	shader_color = _shader_color[0];
	gl_Position = pr_matrix * (gl_in[0].gl_Position + vec4(0.0, h, 0.0, 0.0));
	EmitVertex();
	EndPrimitive();

	
	//shader_uv = vec2(0.0, 1.0);
	//shader_id = _shader_id[0];
	//shader_color = _shader_color[0];
	//gl_Position = pr_matrix * gl_in[0].gl_Position;
	//EmitVertex();

}