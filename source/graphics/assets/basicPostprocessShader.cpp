#include "basicPostprocessShader.h"



namespace oe::graphics {

	const std::string vertsource = std::string() +
		"#version 400 core" +
		"layout(location = 0) in vec3 vertex_pos;" +
		"out vec2 UV;" +
		"void main() {" +
		"	gl_Position = vec4(vertex_pos, 1);" +
		"	UV = (vertex_pos.xy + vec2(1.0, 1.0)) / 2.0;" +
		"}";

	const std::string fragsource = std::string() +
		"#version 330 core\n" +
		"\n" +
		"out vec4 color;\n" +
		"\n" +
		"in vec2 UV;\n" +
		"\n" +
		"uniform sampler2D unif_texture;\n" +
		"uniform int unif_effect;\n" +
		"uniform float unif_t;\n" +
		"\n" +
		"float gauss_kernel[] = float[](0.035822f, 0.05879f, 0.086425f, 0.113806f, 0.13424f, 0.141836f, 0.13424f, 0.113806f, 0.086425f, 0.05879f, 0.035822f);\n" +
		"\n" +
		"float edge_kernel[] = float[](\n" +
		"-1, -1, -1,\n" +
		"-1,  8, -1,\n" +
		"-1, -1, -1\n" +
		");\n" +
		"\n" +
		"float pixelSizeX;\n" +
		"float pixelSizeY;\n" +
		"\n" +
		"float rand(vec2 co) {\n" +
		"return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);\n" +
		"}\n" +
		"\n" +
		"void main()\n" +
		"{\n" +
		"pixelSizeX = 1.0 / textureSize(unif_texture, 0).x;\n" +
		"pixelSizeY = 1.0 / textureSize(unif_texture, 0).y;\n" +
		"if (unif_effect == 1) {\n" +
		"color = vec4(0.0);\n" +
		"\n" +
		"//Horitzontal gaussian blur\n" +
		"for (int i = -5; i < 6; i++) {\n" +
		"float ioff = pixelSizeX * i;\n" +
		"color += texture(unif_texture, vec2(UV.x + ioff, UV.y)) * gauss_kernel[i + 5];\n" +
		"}\n" +
		"}\n" +
		"else if (unif_effect == 2) {\n" +
		"color = vec4(0.0);\n" +
		"\n" +
		"//Vertical gaussian blur\n" +
		"for (int i = -5; i < 6; i++) {\n" +
		"float ioff = pixelSizeY * i;\n" +
		"color += texture(unif_texture, vec2(UV.x, UV.y + ioff)) * gauss_kernel[i + 5];\n" +
		"}\n" +
		"}\n" +
		"else if (unif_effect == 3) {\n" +
		"//Inverted colors\n" +
		"color = vec4(1.0, 1.0, 1.0, 2.0) - texture(unif_texture, UV);\n" +
		"}\n" +
		"else if (unif_effect == 4) {\n" +
		"//Edge detection\n" +
		"color += texture(unif_texture, vec2(UV.x - pixelSizeX, UV.y - pixelSizeY));\n" +
		"color += texture(unif_texture, vec2(UV.x - pixelSizeX, UV.y));\n" +
		"color += texture(unif_texture, vec2(UV.x - pixelSizeX, UV.y + pixelSizeY));\n" +
		"color += 0;// texture(unif_texture, vec2(UV.x, UV.y - pixelSizeY));\n" +
		"color += 0;// texture(unif_texture, vec2(UV.x, UV.y));\n" +
		"color += 0;// texture(unif_texture, vec2(UV.x, UV.y + pixelSizeY));\n" +
		"color += -texture(unif_texture, vec2(UV.x + pixelSizeX, UV.y - pixelSizeY));\n" +
		"color += -texture(unif_texture, vec2(UV.x + pixelSizeX, UV.y));\n" +
		"color += -texture(unif_texture, vec2(UV.x + pixelSizeX, UV.y + pixelSizeY));\n" +
		"\n" +
		"//Black and white\n" +
		"color.x = color.y = color.z = (color.x + color.y + color.z) / 3;\n" +
		"// = color.x;\n" +
		"//color.z = color.x;\n" +
		"}\n" +
		"else if (unif_effect == 5) {\n" +
		"// Trippy\n" +
		"color = texture(unif_texture, UV) * (sin(50 * (UV.x) + unif_t) + 1.0) / 2.0 * (sin(50 * (UV.y) + unif_t) + 1.0) / 2.0;\n" +
		"color.a = 1.0;\n" +
		"}\n" +
		"else {\n" +
		"//Normal\n" +
		"color = texture(unif_texture, UV);\n" +
		"}\n" +
		"}";

	BasicPostprocessShader::BasicPostprocessShader() : Shader("Asset:BasicPostprocessShader") {
		try {
			load(vertsource, fragsource);
		}
		catch (ShaderException & se) {
			// This shouldn't fail
			spdlog::error(se.what());
		}
	}

}