#version 330 core
layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec4 aColor; 
//layout (location = 2) in vec2 aTexturePos;

out vec4 color;
//out vec2 texturePos;

uniform mat4 pr_matrix;
uniform mat4 vw_matrix = mat4(1.0);
uniform mat4 ml_matrix = mat4(1.0);

void main()
{
	gl_Position = pr_matrix * ml_matrix * vw_matrix * aPosition;
	color = aColor;
	//texturePos = aTexturePos;
}