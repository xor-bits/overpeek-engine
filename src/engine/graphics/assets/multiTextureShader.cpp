#include "multiTextureShader.h"



namespace oe::graphics {

	constexpr char* vertsource = R"shader(
		#version 330 core
		layout(location = 0) in vec3 vertex_pos;
		layout(location = 1) in vec2 texture_uv;
		layout(location = 2) in float texture_id;
		layout(location = 3) in vec4 vertex_color;

		out vec2 shader_uv;
		out vec4 shader_color;
		flat out int shader_id;
		
		uniform mat4 pr_matrix = mat4(1.0);
		uniform mat4 ml_matrix = mat4(1.0);
		uniform mat4 vw_matrix = mat4(1.0);
		
		void main()
		{
			mat4 mvp = pr_matrix * vw_matrix * ml_matrix;
			gl_Position = mvp * vec4(vertex_pos.x, vertex_pos.y, vertex_pos.z, 1.0f);
			shader_uv = texture_uv;
			shader_id = int(floor(texture_id));
			shader_color = vertex_color;
		}
	)shader";

	constexpr char* fragsource = R"shader(
		#version 330 core
		in vec2 shader_uv;
		in vec4 shader_color;
		flat in int shader_id;
		
		layout(location = 0) out vec4 color;
		
		uniform sampler2DArray tex;
		uniform int usetex = 1;
		
		void main()
		{
			if (usetex != 0) color = texture(tex, vec3(shader_uv, shader_id)) * shader_color;
			else color = shader_color;
		}
	)shader";

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