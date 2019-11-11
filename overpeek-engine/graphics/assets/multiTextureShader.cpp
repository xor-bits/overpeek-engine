#include "multiTextureShader.h"

namespace oe {

	const std::string vertsource = std::string() +
		"#version 330 core\n" +
		"layout(location = 0) in vec3 vertex_pos;\n" +
		"layout(location = 1) in vec2 texture_uv;\n" +
		"layout(location = 2) in float texture_id;\n" +
		"layout(location = 3) in vec4 vertex_color;\n" +

		"out vec2 shader_uv;\n" +
		"out vec4 shader_color;\n" +
		"flat out int shader_id;\n" +
		"\n" +
		"uniform mat4 pr_matrix = mat4(1.0);\n" +
		"uniform mat4 ml_matrix = mat4(1.0);\n" +
		"uniform mat4 vw_matrix = mat4(1.0);\n" +
		"\n" +
		"void main()\n" +
		"{\n" +
		"   mat4 mvp = pr_matrix * vw_matrix * ml_matrix;\n" +
		"	gl_Position = mvp * vec4(vertex_pos.x, vertex_pos.y, vertex_pos.z, 1.0f);\n" +
		"	shader_uv = texture_uv;\n" +
		"	shader_id = int(floor(texture_id));\n" +
		"	shader_color = vertex_color;\n" +
		"}\n";

	const std::string fragsource = std::string() +
		"\n" +
		"in vec2 shader_uv;\n" +
		"in vec4 shader_color;\n" +
		"flat in int shader_id;\n" +
		"\n" +
		"layout(location = 0) out vec4 color;\n" +
		"\n" +
		"uniform sampler2DArray tex;\n" +
		"uniform int usetex = 1;\n" +
		"\n" +
		"void main()\n" +
		"{\n" +
		"	if (usetex != 0) color = texture(tex, vec3(shader_uv, shader_id)) * shader_color;\n" +
		"	else color = shader_color;\n" +
		"}\n";

	MultiTextureShader::MultiTextureShader() : Shader("Asset:MultiTextureShader") {
		try {
			load(vertsource, fragsource);
		}
		catch (ShaderException &se) {
			// This shouldn't fail
			spdlog::error(se.what());
		}
	}

}