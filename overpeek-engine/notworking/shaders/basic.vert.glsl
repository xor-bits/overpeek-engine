#version 400 core
layout (location = 0) in vec2 aPosition;

uniform mat4 pr_matrix;
uniform mat4 ml_matrix = mat4(1.0);

void main()
{
	gl_Position = pr_matrix * ml_matrix * vec4(aPosition, 0.0f, 1.0f);
}