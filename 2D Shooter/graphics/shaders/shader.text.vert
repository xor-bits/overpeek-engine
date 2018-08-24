#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texture position>

out vec2 texturePosition;


uniform mat4 pr_matrix;

void main()
{
	gl_Position = pr_matrix * vec4(vertex.xy, 0.0, 1.0);
	texturePosition = vertex.zw;
}