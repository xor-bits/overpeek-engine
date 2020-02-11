#include "instance.hpp"

#include "engine/graphics/interface/window.hpp"
#include "engine/graphics/interface/renderer.hpp"
#include "engine/graphics/interface/shader.hpp"
#include "engine/graphics/interface/texture.hpp"
#include "engine/graphics/interface/framebuffer.hpp"



namespace oe::graphics {


	Instance::Instance(const InstanceInfo& instance_info) 
		: m_instance_info(instance_info)
	{

	}

	Instance::~Instance()
	{

	}

}