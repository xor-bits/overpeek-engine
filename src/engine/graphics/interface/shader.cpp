#include "shader.hpp"



namespace oe::graphics {

	IShader::IShader(const ShaderInfo& shader_info)
		: m_shader_info(shader_info) 
	{
	}

	IShader::~IShader() 
	{

	}

}