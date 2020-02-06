#pragma once

#include <string>
#include <fstream>

#include "logical_device.hpp"
#include "swapchain.hpp"



namespace oe::vulkan {

	static std::vector<char> readFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);
		if (!file.is_open()) {
			errorLog("failed to open {}", filename);
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

	template<class dataType>
	class Shader {
	public:
		vk::PipelineLayout m_pipeline_layout;
		vk::Pipeline m_pipeline;
		
		const LogicalDevice* m_logical_device;
		const SwapChain* m_swap_chain;

	public:
		Shader(const SwapChain* swap_chain) : m_logical_device(swap_chain->m_logical_device), m_swap_chain(swap_chain) {}
		
		void loadFiles(std::string vertex, std::string fragment) {
			// load shader code
			auto vertShaderCode = readFile(vertex);
			auto fragShaderCode = readFile(fragment);

			loadSources(vertShaderCode, fragShaderCode);
		}

		void loadSources(std::vector<char> vertex, std::vector<char> fragment) {
			// shader modules
			vk::ShaderModule vertShaderModule = createShaderModule(m_logical_device->m_logical_device, vertex);
			vk::ShaderModule fragShaderModule = createShaderModule(m_logical_device->m_logical_device, fragment);

			// the actual pipeline stage info (vertex shader)
			vk::PipelineShaderStageCreateInfo vertShaderStageInfo = {};
			vertShaderStageInfo.stage = vk::ShaderStageFlagBits::eVertex;
			vertShaderStageInfo.module = vertShaderModule;
			vertShaderStageInfo.pName = "main"; // starts at - int main() {....
			vertShaderStageInfo.pSpecializationInfo = nullptr; // no defined constant values for optimization

			// pipeline stage info for fragment shader
			vk::PipelineShaderStageCreateInfo fragShaderStageInfo = {};
			fragShaderStageInfo.stage = vk::ShaderStageFlagBits::eFragment;
			fragShaderStageInfo.module = fragShaderModule;
			fragShaderStageInfo.pName = "main"; // starts at - int main() {....

			// combine vertex and fragment shader create infos
			vk::PipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

			// pipeline input
			vk::PipelineVertexInputStateCreateInfo vertexInputInfo = {};
#if true
			auto bindingDescription = dataType::getBindingDescription();
			auto attributeDescriptions = dataType::getAttributeDescriptions();
			vertexInputInfo.vertexBindingDescriptionCount = 1;
			vertexInputInfo.pVertexBindingDescriptions = &bindingDescription; // Optional
			vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
			vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data(); // Optional
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
			viewport.width = static_cast<float>(m_swap_chain->m_extent.width);
			viewport.height = static_cast<float>(m_swap_chain->m_extent.height);
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			// whole viewport visible
			vk::Rect2D scissor = {};
			scissor.offset = { 0, 0 };
			scissor.extent = m_swap_chain->m_extent;

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
			rasterizer.cullMode = vk::CullModeFlagBits::eBack;
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
			pipelineInfo.renderPass = m_swap_chain->m_render_pass;
			pipelineInfo.subpass = 0;
			pipelineInfo.basePipelineHandle = nullptr; // Optional
			pipelineInfo.basePipelineIndex = -1; // Optional

			// graphics pipeline creation
			m_pipeline = m_logical_device->m_logical_device.createGraphicsPipeline(nullptr, pipelineInfo);

			// destroy shader modules (after shader program is created)
			m_logical_device->m_logical_device.destroyShaderModule(fragShaderModule);
			m_logical_device->m_logical_device.destroyShaderModule(vertShaderModule);
		}

		~Shader() {
			m_logical_device->m_logical_device.destroyPipeline(m_pipeline);
			m_logical_device->m_logical_device.destroyPipelineLayout(m_pipeline_layout);
		}
	};

}