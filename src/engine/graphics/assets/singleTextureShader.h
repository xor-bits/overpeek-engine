#pragma once

#include "engine/graphics/shader.h"



namespace oe::graphics {

	class SingleTextureShader {
	private:
		static constexpr char* vertsource = R"shader(
			#version 330

			layout(location = 0) in vec2 vertex_pos;
			layout(location = 1) in vec2 texture_uv;
			layout(location = 2) in vec4 vertex_color;
			
			out vec2 shader_uv;
			out vec4 shader_color;
			
			uniform mat4 pr_matrix = mat4(1.0);
			uniform mat4 ml_matrix = mat4(1.0);
			uniform mat4 vw_matrix = mat4(1.0);
			
			void main()
			{
				mat4 mvp = pr_matrix * vw_matrix * ml_matrix;
				gl_Position = mvp * vec4(vertex_pos, 0.0, 1.0);
				
				shader_uv = texture_uv;
				shader_color = vertex_color;
			}
		)shader";

		static constexpr char* fragsource = R"shader(
			#version 330
		
			in vec2 shader_uv;
			in vec4 shader_color;
			
			layout(location = 0) out vec4 color;
			
			uniform sampler2D tex;
			uniform int usetex = 1;
			
			void main()
			{
				if (usetex != 0) color = texture(tex, vec2(shader_uv)) * shader_color;
				else color = shader_color;
			}
		)shader";

	public:
		static ShaderInfo singleTextureShader() {
			ShaderStageInfo vertex = {};
			vertex.source_is_filepath = false;
			vertex.source = vertsource;
			vertex.stage = oe::vertex_shader;

			ShaderStageInfo fragment = {};
			fragment.source_is_filepath = false;
			fragment.source = fragsource;
			fragment.stage = oe::fragment_shader;

			ShaderInfo info = {};
			info.name = "Asset:SingleTextureShader";
			info.shader_stages = {
				vertex, fragment
			};

			return info;
		}

		// void modelMatrix(glm::mat4 matrix) const {
		// 	setUniformMat4("ml_matrix", matrix);
		// }
		// 
		// void viewMatrix(glm::mat4 matrix) const {
		// 	setUniformMat4("vw_matrix", matrix);
		// }
		// 
		// void projectionMatrix(glm::mat4 matrix) const {
		// 	setUniformMat4("pr_matrix", matrix);
		// }
		// 
		// void useTexture(bool use_tex) const {
		// 	setUniform1i("usetex", use_tex ? 1 : 0);
		// }

	};

	typedef SingleTextureShader SpriteShader;
}