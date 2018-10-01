#version 400 core

in vec2 fragPosition;

uniform vec4 color;

void main()
{
	gl_FragColor = color;
}