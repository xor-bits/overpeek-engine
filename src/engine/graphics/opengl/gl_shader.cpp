#include "gl_shader.hpp"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/utility/fileio.hpp"
#include "engine/engine.hpp"



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
		oe_debug_call("gl_shader");

		// Shader program
		p_shader_program = glCreateProgram();

		std::vector<GLuint> modules;
		for (auto& stage : m_shader_info.shader_stages)
		{
			// stage type
			uint32_t stage_id;
			switch (stage.stage)
			{
			case oe::shader_stages::vertex_shader:
				stage_id = GL_VERTEX_SHADER;
				break;
			case oe::shader_stages::tesselation_control_shader:
				stage_id = GL_TESS_CONTROL_SHADER;
				break;
			case oe::shader_stages::tesselation_evaluation_shader:
				stage_id = GL_TESS_EVALUATION_SHADER;
				break;
			case oe::shader_stages::geometry_shader:
				stage_id = GL_GEOMETRY_SHADER;
				break;
			case oe::shader_stages::fragment_shader:
				stage_id = GL_FRAGMENT_SHADER;
				break;
			case oe::shader_stages::compute_shader:
				stage_id = GL_COMPUTE_SHADER;
				break;
			}

			// shader compilation
			GLuint shader_module = loadShader(stage.source, stage_id);
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

	GLShader::~GLShader() {
		glDeleteProgram(p_shader_program);
	}



	void GLShader::bind() const { glUseProgram(p_shader_program); }
	void GLShader::unbind() const { glUseProgram(0); }

	int32_t GLShader::getUniformLocation(const std::string& name) {
		auto iterator = m_uniform_lookup_table.find(name);

		if (iterator != m_uniform_lookup_table.end()) {
			return iterator->second;
		}
		else {
			int32_t location = glGetUniformLocation(p_shader_program, name.c_str());
			m_uniform_lookup_table.insert({ name, location });
			return location;
		}
	}
	
	int location(GLShader* shader, const std::string& name) {
		shader->bind();
		
		int location = shader->getUniformLocation(name); 
		if (location == -1)
			oe_error_terminate("uniform {} was not found", name);

		return location;
	}

	void GLShader::setUniform1f(const std::string& name, float value) { glUniform1f(location(this, name), value); }
	void GLShader::setUniform2f(const std::string& name, const glm::fvec2& value) { glUniform2f(location(this, name), value.x, value.y); }
	void GLShader::setUniform3f(const std::string& name, const glm::fvec3& value) { glUniform3f(location(this, name), value.x, value.y, value.z); }
	void GLShader::setUniform4f(const std::string& name, const glm::fvec4& value) { glUniform4f(location(this, name), value.x, value.y, value.z, value.w); }
	void GLShader::setUniform1i(const std::string& name, int value) { glUniform1i(location(this, name), value); }
	void GLShader::setUniform2i(const std::string& name, const glm::ivec2& value) { glUniform2i(location(this, name), value.x, value.y); }
	void GLShader::setUniform3i(const std::string& name, const glm::ivec3& value) { glUniform3i(location(this, name), value.x, value.y, value.z); }
	void GLShader::setUniform4i(const std::string& name, const glm::ivec4& value) { glUniform4i(location(this, name), value.x, value.y, value.z, value.w); }
	void GLShader::setUniformMat4(const std::string& name, const glm::mat4& value) { glUniformMatrix4fv(location(this, name), 1, GL_FALSE, glm::value_ptr(value)); }
}