#version 400 core

//#define LENS

out vec4 color;

in vec2 UV;

uniform sampler2D unif_texture;
uniform int unif_effect;

uniform int unif_lens;

float gauss_kernel[] = float[](0.035822f, 0.05879f, 0.086425f, 0.113806f, 0.13424f, 0.141836f, 0.13424f, 0.113806f, 0.086425f, 0.05879f, 0.035822f);

float edge_kernel[] = float[](
	-1, -1, -1,
	-1,  8, -1,
	-1, -1, -1
);

float pixelSizeX;
float pixelSizeY;

void main()
{
	pixelSizeX = 1.0 / textureSize(unif_texture, 0).x;
	pixelSizeY = 1.0 / textureSize(unif_texture, 0).y;
	if (unif_effect == 1) {
		color = vec4(0.0);

		//Horitzontal gaussian blur
		for (int i = -5; i < 6; i++) {
			float ioff = pixelSizeX * i;
			color += texture(unif_texture, vec2(UV.x + ioff, UV.y)) * gauss_kernel[i + 5];
		}
	}
	else if (unif_effect == 2) {
		color = vec4(0.0);

		//Vertical gaussian blur
		for (int i = -5; i < 6; i++) {
			float ioff = pixelSizeY * i;
			color += texture(unif_texture, vec2(UV.x, UV.y + ioff)) * gauss_kernel[i + 5];
		}
	}
	else if (unif_effect == 3) {
		//Inverted colors
		color = vec4(1.0, 1.0, 1.0, 2.0) - texture(unif_texture, UV);
	}
	else if (unif_effect == 4) {
		//Edge detection
		color += texture(unif_texture, vec2(UV.x - pixelSizeX, UV.y - pixelSizeY));
		color += texture(unif_texture, vec2(UV.x - pixelSizeX, UV.y));
		color += texture(unif_texture, vec2(UV.x - pixelSizeX, UV.y + pixelSizeY));
		color += 0;// texture(unif_texture, vec2(UV.x, UV.y - pixelSizeY));
		color += 0;// texture(unif_texture, vec2(UV.x, UV.y));
		color += 0;// texture(unif_texture, vec2(UV.x, UV.y + pixelSizeY));
		color += -texture(unif_texture, vec2(UV.x + pixelSizeX, UV.y - pixelSizeY));
		color += -texture(unif_texture, vec2(UV.x + pixelSizeX, UV.y));
		color += -texture(unif_texture, vec2(UV.x + pixelSizeX, UV.y + pixelSizeY));

		//Black and white
		color.x = color.y = color.z = (color.x + color.y + color.z) / 3;
		// = color.x;
		//color.z = color.x;
	}
	else {
		//Normal
		color = texture(unif_texture, UV);
	}
}