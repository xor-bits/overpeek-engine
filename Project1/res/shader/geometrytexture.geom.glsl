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
uniform mat4 vw_matrix = mat4(1.0);

void main() {
	mat4 mvp = pr_matrix * vw_matrix;

	float w = _shader_size[0].x;
	float h = _shader_size[0].y;

	vec3 cameraRight = vec3(vw_matrix[0][0], vw_matrix[1][0], vw_matrix[2][0]);
	vec3 cameraUp = vec3(vw_matrix[0][1], vw_matrix[1][1], vw_matrix[2][1]);
	vec3 pos = gl_in[0].gl_Position.xyz;
	vec4 v1 = mvp * vec4(pos + cameraRight * -0.5 * w + cameraUp * -0.5 * h, 1.0);
	vec4 v2 = mvp * vec4(pos + cameraRight * -0.5 * w + cameraUp *  0.5 * h, 1.0);
	vec4 v3 = mvp * vec4(pos + cameraRight *  0.5 * w + cameraUp *  0.5 * h, 1.0);
	vec4 v4 = mvp * vec4(pos + cameraRight *  0.5 * w + cameraUp * -0.5 * h, 1.0);

	//Triangle 0
	shader_uv = vec2(0.0, 0.0);
	shader_id = _shader_id[0];
	shader_color = _shader_color[0];
	gl_Position = v1;
	EmitVertex();

	shader_uv = vec2(1.0, 1.0);
	shader_id = _shader_id[0];
	shader_color = _shader_color[0];
	gl_Position = v3;
	EmitVertex();
    
	shader_uv = vec2(1.0, 0.0);
	shader_id = _shader_id[0];
	shader_color = _shader_color[0];
	gl_Position = v2;
	EmitVertex();
	EndPrimitive();
    
	//Triangle 1
	shader_uv = vec2(0.0, 0.0);
	shader_id = _shader_id[0];
	shader_color = _shader_color[0];
	gl_Position = v1;
	EmitVertex();

	shader_uv = vec2(0.0, 1.0);
	shader_id = _shader_id[0];
	shader_color = _shader_color[0];
	gl_Position = v4;
	EmitVertex();
    
	shader_uv = vec2(1.0, 1.0);
	shader_id = _shader_id[0];
	shader_color = _shader_color[0];
	gl_Position = v3;
	EmitVertex();
	EndPrimitive();

	
	//shader_uv = vec2(0.0, 1.0);
	//shader_id = _shader_id[0];
	//shader_color = _shader_color[0];
	//gl_Position = pr_matrix * gl_in[0].gl_Position;
	//EmitVertex();

}