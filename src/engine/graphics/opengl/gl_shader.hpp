#pragma once

#include "engine/graphics/interface/shader.hpp"

#include <unordered_map>
#include <cstdint>
#include <string>



namespace oe::graphics {

	class StorageBuffer;
	class GLShader : public IShader {
	private:
		uint32_t p_shader_program;
		std::unordered_map<std::string, int32_t> m_uniform_lookup_table;

		unsigned int loadShader(const std::string_view& name, const std::string_view& source, unsigned int shadertype);
		void shaderLog(const std::string_view& name, unsigned int shader, unsigned int type) const;
		void programLog(const std::string_view& name, int program, unsigned int type) const;

	public:
		GLShader(const ShaderInfo& shader_info);
		~GLShader();

		virtual void bind() const override;
		virtual void unbind() const override;

		// compute shader only
		void dispatchCompute(glm::vec<3, size_t> work_group_count);
		glm::vec<3, size_t> workGroupSize();

		void bindSSBO(const std::string& block_name, const StorageBuffer* buffer, size_t binding);
		void unbindSSBO(const std::string& block_name, const StorageBuffer* buffer, size_t binding);

		// uniforms
		virtual int32_t getUniformLocation(const std::string& name) override;

		virtual void setUniform1f(const std::string& name, float value) override;
		virtual void setUniform2f(const std::string& name, const glm::fvec2& value) override;
		virtual void setUniform3f(const std::string& name, const glm::fvec3& value) override;
		virtual void setUniform4f(const std::string& name, const glm::fvec4& value) override;
		
		virtual void setUniform1i(const std::string& name, int value) override;
		virtual void setUniform2i(const std::string& name, const glm::ivec2& value) override;
		virtual void setUniform3i(const std::string& name, const glm::ivec3& value) override;
		virtual void setUniform4i(const std::string& name, const glm::ivec4& value) override;

		virtual void setUniformMat4(const std::string& name, const glm::mat4& value) override;
	};

}