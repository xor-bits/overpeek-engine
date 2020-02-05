#include "gl_shader.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/utility/filereader.h"
#include "engine/engine.h"



namespace oe::graphics {

	unsigned int GLShader::loadShader(const std::string& source, unsigned int shadertype) {
		//Load and compile
		const char* shaderChar = source.c_str();

		GLuint shader;
		shader = glCreateShader(shadertype);
		glShaderSource(shader, 1, &shaderChar, NULL);
		glCompileShader(shader);

		//Get errors
		shaderLog(shader, GL_COMPILE_STATUS);
		return shader;
	}

	void GLShader::shaderLog(unsigned int shader, unsigned int type) const {
		int success = false;
		glGetShaderiv(shader, type, &success);
		if (!success)
		{
			GLsizei infoLogLength = 512;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* infoLog = new char[infoLogLength]();
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);

			oe_error_terminate("{}: shader compilation: \n{}", m_shader_info.name, infoLog);
			delete[] infoLog;
		}
	}

	void GLShader::programLog(int program, unsigned int type) const {
		int success;
		glGetProgramiv(program, type, &success);
		if (!success)
		{
			GLsizei infoLogLength = 512;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* infoLog = new char[infoLogLength]();
			glGetProgramInfoLog(program, 512, nullptr, infoLog);

			oe_error_terminate("{}: program linking: \n{}", m_shader_info.name, infoLog);
			delete[] infoLog;
		}
	}



	GLShader::GLShader(const ShaderInfo& shader_info)
		: Shader::Shader(shader_info) 
	{
		// Shader program
		p_shader_program = glCreateProgram();

		std::vector<GLuint> modules;
		for (auto& stage : shader_info.shader_stages)
		{
			// sources
			const char* source = stage.source.c_str();
			if (stage.source_is_filepath) {
				source = oe::utils::readFile(source).c_str();
			}

			// stage type
			uint32_t stage_id;
			switch (stage.stage)
			{
			case vertex_shader:
				stage_id = GL_VERTEX_SHADER;
				break;
			case tesselation_shader:
				stage_id = GL_TESS_CONTROL_SHADER;
				break;
			case geometry_shader:
				stage_id = GL_GEOMETRY_SHADER;
				break;
			case fragment_shader:
				stage_id = GL_FRAGMENT_SHADER;
				break;
			case compute_shader:
				stage_id = GL_COMPUTE_SHADER;
				break;
			}

			// shader compilation
			GLuint shader_module = loadShader(source, stage_id);
			modules.push_back(shader_module);

			// attach shader stage
			glAttachShader(p_shader_program, shader_module);
		}

		// Get shader program linking error
		glLinkProgram(p_shader_program);
		programLog(p_shader_program, GL_LINK_STATUS);

		// Free up data
		for (GLuint shader_module : modules) 
		{
			glDeleteShader(shader_module);
		}
	}



	void GLShader::bind() const { glUseProgram(p_shader_program); }
	void GLShader::unbind() const { glUseProgram(0); }

	int GLShader::getUniformLocation(std::string name) const { return glGetUniformLocation(p_shader_program, name.c_str()); }

	void GLShader::setUniform1f(std::string name, float value) const { bind(); glUniform1f(getUniformLocation(name), value); }
	void GLShader::setUniform2f(std::string name, glm::fvec2& value) const { bind(); glUniform2f(getUniformLocation(name), value.x, value.y); }
	void GLShader::setUniform3f(std::string name, glm::fvec3& value) const { bind(); glUniform3f(getUniformLocation(name), value.x, value.y, value.z); }
	void GLShader::setUniform4f(std::string name, glm::fvec4& value) const { bind(); glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w); }
	void GLShader::setUniform1i(std::string name, int value) const { bind(); glUniform1i(getUniformLocation(name), value); }
	void GLShader::setUniform2i(std::string name, glm::ivec2& value) const { bind(); glUniform2i(getUniformLocation(name), value.x, value.y); }
	void GLShader::setUniform3i(std::string name, glm::ivec3& value) const { bind(); glUniform3i(getUniformLocation(name), value.x, value.y, value.z); }
	void GLShader::setUniform4i(std::string name, glm::ivec4& value) const { bind(); glUniform4i(getUniformLocation(name), value.x, value.y, value.z, value.w); }
	void GLShader::setUniformMat4(std::string name, glm::mat4& value) const { bind(); glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value)); }
}