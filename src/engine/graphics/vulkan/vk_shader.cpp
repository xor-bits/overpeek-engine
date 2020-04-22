#include "vk_shader.hpp"
#ifdef BUILD_VULKAN
#include "vk_support.hpp"
#include "vk_window.hpp"
#include "vk_swapchain.hpp"
#include "buffers/vk_command_pool.hpp"
#include "engine/engine.hpp"



namespace oe::graphics {

	static std::vector<char> readFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);
		if (!file.is_open()) {
			oe_error_terminate("failed to open {}", filename);
		}

		// get size + create buffer
		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		// read data
		file.seekg(0);
		file.read(buffer.data(), fileSize);

		// close the file
		file.close();

		return buffer;
	}

	shaderc_shader_kind shader_kind(oe::shader_stages stage) {
		switch (stage)
		{
		case oe::shader_stages::vertex_shader:
			return shaderc_shader_kind::shaderc_vertex_shader;
			break;
		case oe::shader_stages::tesselation_control_shader:
			return shaderc_shader_kind::shaderc_tess_control_shader;
			break;
		case oe::shader_stages::tesselation_evaluation_shader:
			return shaderc_shader_kind::shaderc_tess_evaluation_shader;
			break;
		case oe::shader_stages::geometry_shader:
			return shaderc_shader_kind::shaderc_geometry_shader;
			break;
		case oe::shader_stages::fragment_shader:
			return shaderc_shader_kind::shaderc_fragment_shader;
			break;
		case oe::shader_stages::compute_shader:
			return shaderc_shader_kind::shaderc_compute_shader;
			break;
		}
	}

	vk::ShaderStageFlagBits shader_flagbits(oe::shader_stages stage) {
		switch (stage)
		{
		case oe::shader_stages::vertex_shader:
			return vk::ShaderStageFlagBits::eVertex;
			break;
		case oe::shader_stages::tesselation_control_shader:
			return vk::ShaderStageFlagBits::eTessellationControl;
			break;
		case oe::shader_stages::tesselation_evaluation_shader:
			return vk::ShaderStageFlagBits::eTessellationEvaluation;
			break;
		case oe::shader_stages::geometry_shader:
			return vk::ShaderStageFlagBits::eGeometry;
			break;
		case oe::shader_stages::fragment_shader:
			return vk::ShaderStageFlagBits::eFragment;
			break;
		case oe::shader_stages::compute_shader:
			return vk::ShaderStageFlagBits::eCompute;
			break;
		}
	}

	std::string shader_module_name(std::string name, oe::shader_stages stage) {
		switch (stage)
		{
		case oe::shader_stages::vertex_shader:
			return name + "." + "vertex";
			break;
		case oe::shader_stages::tesselation_control_shader:
			return name + "." + "tess_control";
			break;
		case oe::shader_stages::tesselation_evaluation_shader:
			return name + "." + "tess_evaluation";
			break;
		case oe::shader_stages::geometry_shader:
			return name + "." + "geometry";
			break;
		case oe::shader_stages::fragment_shader:
			return name + "." + "fragment";
			break;
		case oe::shader_stages::compute_shader:
			return name + "." + "compute";
			break;
		}
	}

	VKShader::VKShader(const VKWindow* window, const ShaderInfo& shader_info)
		: Shader::Shader(shader_info)
		, m_window(window)
		, m_logical_device(window->m_logical_device)
	{
		std::vector<vk::ShaderModule> modules;
		std::vector<vk::PipelineShaderStageCreateInfo> module_infos;
		for (auto& stage : m_shader_info.shader_stages)
		{
			shaderc::Compiler compiler = shaderc::Compiler();
			shaderc::CompileOptions options = shaderc::CompileOptions();
			options.SetOptimizationLevel(shaderc_optimization_level::shaderc_optimization_level_performance);
			auto result = compiler.CompileGlslToSpv(stage.source, shader_kind(stage.stage), shader_module_name(shader_info.name, stage.stage).c_str(), options);
			size_t code_size = (result.end() - result.begin()) * sizeof(uint32_t);

			if (result.GetNumErrors() != 0) {
				oe_error_terminate("Shader ({}) compilation failed: {}", shader_info.name, result.GetErrorMessage());
			}

			// stage type
			vk::ShaderStageFlagBits stage_id = shader_flagbits(stage.stage);

			// shader module
			vk::ShaderModuleCreateInfo createInfo = {};
			createInfo.codeSize = code_size;
			createInfo.pCode = result.begin();
			vk::ShaderModule shader_module = m_logical_device->m_logical_device.createShaderModule(createInfo);

			// the actual pipeline stage info
			vk::PipelineShaderStageCreateInfo shaderStageInfo = {};
			shaderStageInfo.stage = stage_id;
			shaderStageInfo.module = shader_module;
			shaderStageInfo.pName = "main"; // starts at - int main() {....
			shaderStageInfo.pSpecializationInfo = nullptr; // no defined constant values for optimization

			modules.push_back(shader_module);
			module_infos.push_back(shaderStageInfo);
		}

		// combine vertex and fragment shader create infos
		vk::PipelineShaderStageCreateInfo *shaderStages = module_infos.data();

		// pipeline input
		vk::PipelineVertexInputStateCreateInfo vertexInputInfo = {};
#if true
		auto bindingDescription = VKBuffer::getBindingDescription();
		auto attributeDescriptions = VKBuffer::getAttributeDescriptions();
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
#else
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional
#endif

			// assembler
		vk::PipelineInputAssemblyStateCreateInfo inputAssembly = {};
		inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
		inputAssembly.primitiveRestartEnable = VK_FALSE; // no breaking out of triangle_strip or from anything like that

		// set up viewport size
		vk::Viewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_window->m_swapchain->m_extent.width);
		viewport.height = static_cast<float>(m_window->m_swapchain->m_extent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		// whole viewport visible
		vk::Rect2D scissor = {};
		scissor.offset = { 0, 0 };
		scissor.extent = m_window->m_swapchain->m_extent;

		// pipeline viewport stage
		vk::PipelineViewportStateCreateInfo viewportState = {};
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		// pipeline rasterizer stage
		vk::PipelineRasterizationStateCreateInfo rasterizer = {};
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE; // would discard any output to framebuffer
		rasterizer.polygonMode = vk::PolygonMode::eFill; // glPolygonMode()
		rasterizer.lineWidth = 1.0f; // over 1.0f, would need wideLines GPU feature
		rasterizer.cullMode = vk::CullModeFlagBits::eNone;
		rasterizer.frontFace = vk::FrontFace::eClockwise;
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f; // Optional
		rasterizer.depthBiasClamp = 0.0f; // Optional
		rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

		// multisampling (one sample for now)
		vk::PipelineMultisampleStateCreateInfo multisampling = {};
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
		multisampling.minSampleShading = 1.0f; // Optional
		multisampling.pSampleMask = nullptr; // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE; // Optional

		// color blending or framebuffer color assigning
		vk::PipelineColorBlendAttachmentState colorBlendAttachment = {};
		colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eOne; // Optional
		colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eZero; // Optional
		colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd; // Optional
		colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne; // Optional
		colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero; // Optional
		colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd; // Optional

		// color blend create info
		vk::PipelineColorBlendStateCreateInfo colorBlending = {};
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = vk::LogicOp::eCopy; // Optional
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f; // Optional
		colorBlending.blendConstants[1] = 0.0f; // Optional
		colorBlending.blendConstants[2] = 0.0f; // Optional
		colorBlending.blendConstants[3] = 0.0f; // Optional

		// pipeline layout creation info
		vk::PipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.setLayoutCount = 0; // Optional
		pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
		pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
		pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

		// create the pipeline layout
		m_pipeline_layout = m_logical_device->m_logical_device.createPipelineLayout(pipelineLayoutInfo);

		// final graphics pipeline info
		vk::GraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = nullptr; // Optional
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = nullptr; // Optional
		pipelineInfo.layout = m_pipeline_layout; // pipeline layout
		pipelineInfo.renderPass = m_window->m_swapchain->m_render_pass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = nullptr; // Optional
		pipelineInfo.basePipelineIndex = -1; // Optional

		// graphics pipeline creation
		m_pipeline = m_logical_device->m_logical_device.createGraphicsPipeline(nullptr, pipelineInfo);

		// Free up data
		for (vk::ShaderModule shader_module : modules)
		{
			m_logical_device->m_logical_device.destroyShaderModule(shader_module);
		}
	}

	VKShader::~VKShader() {
		m_logical_device->m_logical_device.waitIdle();
		m_logical_device->m_logical_device.destroyPipeline(m_pipeline);
		m_logical_device->m_logical_device.destroyPipelineLayout(m_pipeline_layout);
	}



	// needs implementation/fixing
	void VKShader::bind() const
	{
		m_window->m_command_pool->bindShader(this);
	}
	void VKShader::unbind() const
	{
	}
	int VKShader::getUniformLocation(std::string name) const
	{
		return 0;
	}
	void VKShader::setUniform1f(std::string name, float value) const	  
	{
	}
	void VKShader::setUniform2f(std::string name, glm::fvec2& value) const
	{
	}
	void VKShader::setUniform3f(std::string name, glm::fvec3& value) const
	{
	}
	void VKShader::setUniform4f(std::string name, glm::fvec4& value) const
	{
	}
	void VKShader::setUniform1i(std::string name, int value) const
	{
	}
	void VKShader::setUniform2i(std::string name, glm::ivec2& value) const
	{
	}
	void VKShader::setUniform3i(std::string name, glm::ivec3& value) const
	{
	}
	void VKShader::setUniform4i(std::string name, glm::ivec4& value) const
	{
	}
	void VKShader::setUniformMat4(std::string name, glm::mat4& value) const
	{
	}

}
#endif