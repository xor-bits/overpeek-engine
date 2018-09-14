#version 400 core
layout (location = 0) in vec3 aPosition;

out vec3 fragPosition;

uniform mat4 pr_matrix;
uniform mat4 vw_matrix = mat4(1.0);
uniform mat4 ml_matrix = mat4(1.0);

void main()
{
	gl_Position = pr_matrix * vw_matrix * ml_matrix * vec4(aPosition, 1.0f);
	fragPosition = aPosition;
}