#version 330 core
layout (location = 0) in vec2 aPosition;

out vec4 color;

uniform mat4 pr_matrix;
uniform mat4 ml_matrix = mat4(1.0);

uniform vec4 testColor;

void main()
{
	gl_Position = pr_matrix * ml_matrix * vec4(aPosition, 0.0f, 1.0f);
	color = testColor;
}