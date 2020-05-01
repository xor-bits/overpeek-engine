#pragma once

#include "engine/graphics/interface/shader.hpp"

#include <unordered_map>
#include <cstdint>
#include <string>



namespace oe::graphics {

	class GLShader : public Shader {
	private:
		uint32_t p_shader_program;
		std::unordered_map<std::string, int32_t> m_uniform_lookup_table;

		unsigned int loadShader(const std::string& source, unsigned int shadertype);
		void shaderLog(unsigned int shader, unsigned int type) const;
		void programLog(int program, unsigned int type) const;

	public:
		GLShader(const ShaderInfo& shader_info);
		~GLShader();

		virtual void bind() const override;
		virtual void unbind() const override;

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