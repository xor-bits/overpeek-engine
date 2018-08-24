#version 330 core
out vec4 FragColor;

in vec4 color;
//in vec2 texturePos;

//uniform sampler2D ourTexture;

void main()
{
	//FragColor = texture(ourTexture, texturePos);
	FragColor = color;
}