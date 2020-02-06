#include "instance.h"

#include "engine/graphics/interface/window.h"
#include "engine/graphics/interface/renderer.h"
#include "engine/graphics/interface/shader.h"
#include "engine/graphics/interface/texture.h"
#include "engine/graphics/interface/framebuffer.h"



namespace oe::graphics {


	Instance::Instance(const InstanceInfo& instance_info) 
		: m_instance_info(instance_info)
	{

	}

	Instance::~Instance()
	{

	}

}