#include "gl_shader.hpp"

#include <iostream>

#include "gl_include.hpp"
#include <GLFW/glfw3.h>


#if OE_BUILD_MODE_SHADERC
#include <shaderc/shaderc.hpp>
#include "engine/graphics/interface/shader_common.hpp"
#endif

#include "engine/utility/fileio.hpp"
#include "engine/engine.hpp"
#include "engine/graphics/opengl/buffers/storageBuffer.hpp"
#include "engine/utility/formatted_error.hpp"



namespace oe::graphics
{

	unsigned int GLShader::loadShader(const std::string_view& name, const std::string_view& source, oe::shader_stages shader_stage)
	{
		// stage type
		uint32_t stage_id = GL_VERTEX_SHADER;
		switch (shader_stage)
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
		case oe::shader_stages::none:
			throw oe::utils::formatted_error("invalid shader stage: {}", static_cast<size_t>(shader_stage));
		}
		
		//Load and compile
		GLuint shader;
		shader = glCreateShader(stage_id);
		const char* lines = source.data();
		glShaderSource(shader, 1, &lines, NULL);
		glCompileShader(shader);

		//Get errors
		shaderLog(name, shader, GL_COMPILE_STATUS);
		return shader;
	}

	void GLShader::shaderLog(const std::string_view& name, unsigned int shader, unsigned int type) const
	{
		int success = false;
		glGetShaderiv(shader, type, &success);
		if (!success)
		{
			GLsizei infoLogLength = 512;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
			std::string infoLog; infoLog.resize(infoLogLength);
			glGetShaderInfoLog(shader, 512, nullptr, infoLog.data());

			throw oe::utils::formatted_error("{}: shader compilation: \n{}", name, infoLog);
		}
	}

	void GLShader::programLog(const std::string_view& name, int program, unsigned int type) const
	{
		int success = false;
		glGetProgramiv(program, type, &success);
		if (!success)
		{
			GLsizei infoLogLength = 512;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
			std::string infoLog; infoLog.resize(infoLogLength);
			glGetProgramInfoLog(program, 512, nullptr, infoLog.data());

			throw oe::utils::formatted_error("{}: program linking: \n{}", name, infoLog);
		}
	}



	GLShader::GLShader(const ShaderInfo& shader_info)
		: IShader::IShader(shader_info) 
	{
		// IShader program
		p_shader_program = glCreateProgram();

		std::vector<GLuint> modules;
		for (auto& stage : shader_info.shader_stages)
		{
#ifdef OE_BUILD_MODE_SHADERC // OPTIMIZE GLSL
			// optimization and <#include>:s
			std::unique_ptr<Includer> includer = std::make_unique<Includer>(stage.include_path);

			shaderc::Compiler compiler = shaderc::Compiler();
			shaderc::CompileOptions options = shaderc::CompileOptions();
			if(shader_info.shaderc_optimize)
				options.SetOptimizationLevel(shaderc_optimization_level::shaderc_optimization_level_performance);
			options.SetIncluder(std::move(includer));
			auto result = compiler.PreprocessGlsl(stage.source.data(), shader_kind(stage.stage), shader_module_name(shader_info.name.data(), stage.stage).c_str(), options);

			if (result.GetNumErrors() != 0)
				throw oe::utils::formatted_error("IShader ({}) compilation failed: {}", shader_info.name, result.GetErrorMessage());
			std::string_view preprocessed_glsl = { result.begin(), static_cast<size_t>(std::max<int64_t>(0, std::distance(result.begin(), result.end()))) };
#else  // DON'T OPTIMIZE GLSL
			std::string_view preprocessed_glsl = stage.source;
#endif

			// shader compilation
			GLuint shader_module = loadShader(shader_info.name, preprocessed_glsl, stage.stage);
			modules.push_back(shader_module);

			// attach shader stage
			glAttachShader(p_shader_program, shader_module);
		}

		// Get shader program linking error
		glLinkProgram(p_shader_program);
		programLog(shader_info.name, p_shader_program, GL_LINK_STATUS);

		// Free up data
		for (GLuint shader_module : modules) 
		{
			glDeleteShader(shader_module);
		}
	}

	GLShader::~GLShader()
	{
		glDeleteProgram(p_shader_program);
	}



	void GLShader::bind() const { glUseProgram(p_shader_program); }
	void GLShader::unbind() const { glUseProgram(0); }
	
	void GLShader::dispatchCompute(glm::vec<3, size_t> work_groups)
	{
		glDispatchCompute(work_groups.x, work_groups.y, work_groups.z);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	}

	glm::vec<3, size_t> GLShader::workGroupSize()
	{
		std::array<int32_t, 3> max_work_group_size;
		glGetProgramiv(p_shader_program, GL_COMPUTE_WORK_GROUP_SIZE, max_work_group_size.data());
		return { max_work_group_size[0], max_work_group_size[1], max_work_group_size[2] };
	}

	void GLShader::bindSSBO(const std::string& block_name, const StorageBuffer* buffer, size_t binding)
	{
		buffer->bind();
		buffer->compute(binding);
		uint32_t block_index = 0;
		block_index = glGetProgramResourceIndex(p_shader_program, GL_SHADER_STORAGE_BLOCK, block_name.c_str());
#ifndef __EMSCRIPTEN__
		glShaderStorageBlockBinding(p_shader_program, block_index, binding);
#endif
	}

	void GLShader::unbindSSBO(const StorageBuffer* buffer)
	{
		buffer->unbind();
	}

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
			spdlog::warn("uniform {} was not found", name);

		return location;
	}

	void GLShader::setUniform(const std::string& name, const float value) { glUniform1f(location(this, name), value); }
	void GLShader::setUniform(const std::string& name, const glm::fvec2& value) { glUniform2f(location(this, name), value.x, value.y); }
	void GLShader::setUniform(const std::string& name, const glm::fvec3& value) { glUniform3f(location(this, name), value.x, value.y, value.z); }
	void GLShader::setUniform(const std::string& name, const glm::fvec4& value) { glUniform4f(location(this, name), value.x, value.y, value.z, value.w); }
	void GLShader::setUniform(const std::string& name, const size_t count, const float* values) { glUniform1fv(location(this, name), count, values); }
	void GLShader::setUniform(const std::string& name, const size_t count, const glm::fvec2* values) { glUniform2fv(location(this, name), count, glm::value_ptr(values[0])); }
	void GLShader::setUniform(const std::string& name, const size_t count, const glm::fvec3* values) { glUniform3fv(location(this, name), count, glm::value_ptr(values[0])); }
	void GLShader::setUniform(const std::string& name, const size_t count, const glm::fvec4* values) { glUniform4fv(location(this, name), count, glm::value_ptr(values[0])); }
	
	void GLShader::setUniform(const std::string& name, const int32_t value) { glUniform1i(location(this, name), value); }
	void GLShader::setUniform(const std::string& name, const glm::ivec2& value) { glUniform2i(location(this, name), value.x, value.y); }
	void GLShader::setUniform(const std::string& name, const glm::ivec3& value) { glUniform3i(location(this, name), value.x, value.y, value.z); }
	void GLShader::setUniform(const std::string& name, const glm::ivec4& value) { glUniform4i(location(this, name), value.x, value.y, value.z, value.w); }
	void GLShader::setUniform(const std::string& name, const size_t count, const int32_t* values) { glUniform1iv(location(this, name), count, values); }
	void GLShader::setUniform(const std::string& name, const size_t count, const glm::ivec2* values) { glUniform2iv(location(this, name), count, glm::value_ptr(values[0])); }
	void GLShader::setUniform(const std::string& name, const size_t count, const glm::ivec3* values) { glUniform3iv(location(this, name), count, glm::value_ptr(values[0])); }
	void GLShader::setUniform(const std::string& name, const size_t count, const glm::ivec4* values) { glUniform4iv(location(this, name), count, glm::value_ptr(values[0])); }
	
	void GLShader::setUniform(const std::string& name, const uint32_t value) { glUniform1ui(location(this, name), value); }
	void GLShader::setUniform(const std::string& name, const glm::uvec2& value) { glUniform2ui(location(this, name), value.x, value.y); }
	void GLShader::setUniform(const std::string& name, const glm::uvec3& value) { glUniform3ui(location(this, name), value.x, value.y, value.z); }
	void GLShader::setUniform(const std::string& name, const glm::uvec4& value) { glUniform4ui(location(this, name), value.x, value.y, value.z, value.w); }
	void GLShader::setUniform(const std::string& name, const size_t count, const uint32_t* values) { glUniform1uiv(location(this, name), count, values); }
	void GLShader::setUniform(const std::string& name, const size_t count, const glm::uvec2* values) { glUniform2uiv(location(this, name), count, glm::value_ptr(values[0])); }
	void GLShader::setUniform(const std::string& name, const size_t count, const glm::uvec3* values) { glUniform3uiv(location(this, name), count, glm::value_ptr(values[0])); }
	void GLShader::setUniform(const std::string& name, const size_t count, const glm::uvec4* values) { glUniform4uiv(location(this, name), count, glm::value_ptr(values[0])); }
	
	void GLShader::setUniform(const std::string& name, const glm::mat4& value) { glUniformMatrix4fv(location(this, name), 1, GL_FALSE, glm::value_ptr(value)); }
	void GLShader::setUniform(const std::string& name, const size_t count, const glm::mat4* value) { glUniformMatrix4fv(location(this, name), count, GL_FALSE, glm::value_ptr(value[0])); }
}