#pragma once

#include "engine/graphics/interface/shader.hpp"

#include <unordered_map>
#include <cstdint>
#include <string>



namespace oe::graphics
{

	class StorageBuffer;
	class GLShader : public IShader
	{
	private:
		uint32_t p_shader_program;
		std::unordered_map<std::string, int32_t> m_uniform_lookup_table;

		unsigned int loadShader(const std::string_view& name, const std::string_view& source, unsigned int shadertype);
		void shaderLog(const std::string_view& name, unsigned int shader, unsigned int type) const;
		void programLog(const std::string_view& name, int program, unsigned int type) const;

	public:
		GLShader(const ShaderInfo& shader_info);
		~GLShader();

		void bind() const override;
		void unbind() const override;

		// compute shader only
		void dispatchCompute(glm::vec<3, size_t> work_group_count);
		glm::vec<3, size_t> workGroupSize();

		void bindSSBO(const std::string& block_name, const StorageBuffer* buffer, size_t binding);
		void unbindSSBO(const StorageBuffer* buffer);

		// uniforms
		int32_t getUniformLocation(const std::string& name) override;

		void setUniform(const std::string& name, const float value) override;
		void setUniform(const std::string& name, const glm::fvec2& value) override;
		void setUniform(const std::string& name, const glm::fvec3& value) override;
		void setUniform(const std::string& name, const glm::fvec4& value) override;
		void setUniform(const std::string& name, const size_t count, const float* values) override;
		void setUniform(const std::string& name, const size_t count, const glm::fvec2* values) override;
		void setUniform(const std::string& name, const size_t count, const glm::fvec3* values) override;
		void setUniform(const std::string& name, const size_t count, const glm::fvec4* values) override;
		
		void setUniform(const std::string& name, const int32_t value) override;
		void setUniform(const std::string& name, const glm::ivec2& value) override;
		void setUniform(const std::string& name, const glm::ivec3& value) override;
		void setUniform(const std::string& name, const glm::ivec4& value) override;
		void setUniform(const std::string& name, const size_t count, const int32_t* values) override;
		void setUniform(const std::string& name, const size_t count, const glm::ivec2* values) override;
		void setUniform(const std::string& name, const size_t count, const glm::ivec3* values) override;
		void setUniform(const std::string& name, const size_t count, const glm::ivec4* values) override;
		
		void setUniform(const std::string& name, const uint32_t value) override;
		void setUniform(const std::string& name, const glm::uvec2& value) override;
		void setUniform(const std::string& name, const glm::uvec3& value) override;
		void setUniform(const std::string& name, const glm::uvec4& value) override;
		void setUniform(const std::string& name, const size_t count, const uint32_t* values) override;
		void setUniform(const std::string& name, const size_t count, const glm::uvec2* values) override;
		void setUniform(const std::string& name, const size_t count, const glm::uvec3* values) override;
		void setUniform(const std::string& name, const size_t count, const glm::uvec4* values) override;

		void setUniform(const std::string& name, const glm::mat4& value) override;
		void setUniform(const std::string& name, const size_t count, const glm::mat4* values) override;
	};
}