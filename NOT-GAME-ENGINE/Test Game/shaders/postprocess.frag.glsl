#version 400 core

out vec4 color;

in vec2 UV;

uniform sampler2D renderedTexture;
uniform int unif_effect;

float kernel[] = float[](0.035822,	0.05879,	0.086425,	0.113806,	0.13424,	0.141836,	0.13424,	0.113806,	0.086425,	0.05879,	0.035822);
float pixelSizeX;
float pixelSizeY;

void main()
{
	pixelSizeX = 1.0 / textureSize(renderedTexture, 0).x;
	pixelSizeY = 1.0 / textureSize(renderedTexture, 0).y;
	if (unif_effect == 1) {
		color = vec4(0.0);

		for (int i = -5; i <= 5; i++) {
			float ioff = pixelSizeX * i;
			color += texture(renderedTexture, vec2(UV.x + ioff, UV.y)) * kernel[i + 5];
		}
	}
	else if (unif_effect == 2) {
		color = vec4(0.0);
		
		for (int i = -5; i < 6; i++) {
			float ioff = pixelSizeY * i;
			color += texture(renderedTexture, vec2(UV.x, UV.y + ioff)) * kernel[i + 5];
		}
	}
	else if (unif_effect == 3) {
		color = vec4(1.0, 1.0, 1.0, 2.0) - texture(renderedTexture, UV);
	}
	else {
		color = texture(renderedTexture, UV);
	}
}