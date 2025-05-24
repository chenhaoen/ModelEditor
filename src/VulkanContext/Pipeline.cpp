#include "VulkanContext/VulkanContext.h"
#include "VulkanContext/Pipeline.h"
#include "VulkanContext/LogicalDevice.h"
#include "VulkanContext/Surface.h"
#include "VulkanContext/RenderPass.h"
#include "VulkanContext/DescriptorSetLayout.h"
#include "VulkanContext/Utils.h"

#include "Core/Shader/Shader.h"

namespace {
	static const VkPrimitiveTopology RD_TO_VK_PRIMITIVE[static_cast<int>(RenderPrimitive::RENDER_PRIMITIVE_MAX)] = {
	VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
	VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
	VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY,
	VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
	VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY,
	VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
	VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY,
	VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
	VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY,
	VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
	VK_PRIMITIVE_TOPOLOGY_PATCH_LIST,
	};
}

Pipeline::Pipeline(const PipelineCreateInfo& pipelineCreateInfo, 
	DescriptorSetLayout* descriptorSetLayout)
	: m_descriptorSetLayout(descriptorSetLayout)
	, m_type(pipelineCreateInfo.m_type)
	, m_pipeline(nullptr)
	, m_layout(nullptr)
{
	switch (pipelineCreateInfo.m_type)
	{
	case PipelineType::Graphics:
		createGraphicsPipeline(pipelineCreateInfo,descriptorSetLayout);
		break;
	case PipelineType::Compute:
		createComputePipeline(pipelineCreateInfo, descriptorSetLayout);
		break;
	default:
		break;
	}

	
}

Pipeline::~Pipeline()
{
	vkDestroyPipeline(VulkanContext::getDevice()->getVkDevice(), m_pipeline, nullptr);
	vkDestroyPipelineLayout(VulkanContext::getDevice()->getVkDevice(), m_layout, nullptr);
}

VkPipeline Pipeline::getVkPipeline() const
{
	return m_pipeline;
}

VkPipelineLayout Pipeline::getVkPipelineLayout() const
{
	return m_layout;
}

PipelineType Pipeline::getType() const
{
	return m_type;
}

void Pipeline::createGraphicsPipeline(const PipelineCreateInfo& pipelineCreateInfo, DescriptorSetLayout* descriptorSetLayout)
{
	std::vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos;
	std::vector<VkShaderModule> shaderModules;
	for (auto shader : pipelineCreateInfo.m_shaders)
	{
		auto shaderCode = readFile(shader->getFileName());

		VkShaderModule shaderModule = createShaderModule(shaderCode);

		VkPipelineShaderStageCreateInfo shaderStageInfo{};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = ShaderTypeToVk(shader->getType());
		shaderStageInfo.module = shaderModule;
		shaderStageInfo.pName = shader->getFuncName().data();

		shaderModules.push_back(shaderModule);
		shaderStageCreateInfos.push_back(shaderStageInfo);
	}

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_NONE;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f;          // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f;    // Optional

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f;          // Optional
	multisampling.pSampleMask = nullptr;            // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE;      // Optional

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;             // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;             // Optional

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	std::vector<VkDescriptorSetLayout> setLayouts;
	setLayouts.push_back(m_descriptorSetLayout->getVkDescriptorSetLayout());

	VkPushConstantRange pushConstantRange = {};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; // 指定着色器阶段
	pushConstantRange.offset = 0; // 偏移量
	pushConstantRange.size = sizeof(UniformBufferObject); // 数据大小

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = setLayouts.size(); // Optional
	pipelineLayoutInfo.pSetLayouts = setLayouts.data();    // Optional
	pipelineLayoutInfo.pushConstantRangeCount = 0;         // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr;      // Optional

	VK_CHECK(vkCreatePipelineLayout(VulkanContext::getDevice()->getVkDevice(), &pipelineLayoutInfo, nullptr, &m_layout));

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = shaderStageCreateInfos.size();
	pipelineInfo.pStages = shaderStageCreateInfos.data();

	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f; // Optional
	depthStencil.maxDepthBounds = 1.0f; // Optional
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {}; // Optional
	depthStencil.back = {};  // Optional

	std::vector<VkDynamicState> dynamicStates;

	for (auto type : pipelineCreateInfo.m_dynamicStates)
	{
		switch (type)
		{
		case DynamicStateType::Viewport:
			dynamicStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);
			break;
		case DynamicStateType::Scissor:
			dynamicStates.push_back(VK_DYNAMIC_STATE_SCISSOR);
			break;
		case DynamicStateType::PolygonMode:
			dynamicStates.push_back(VK_DYNAMIC_STATE_POLYGON_MODE_EXT);
			break;
		default:
			break;
		}
	}

	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = RD_TO_VK_PRIMITIVE[static_cast<int>(pipelineCreateInfo.m_renderPrimitive)];
	inputAssembly.primitiveRestartEnable = (pipelineCreateInfo.m_renderPrimitive == RenderPrimitive::RENDER_PRIMITIVE_TRIANGLE_STRIPS_WITH_RESTART_INDEX);

	auto bindingDescription = getBindingDescription();
	auto attributeDescriptions = getAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;

	VkExtent2D swapChainExtent = VulkanContext::getSurface()->getExtent();

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChainExtent.width;
	viewport.height = (float)swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil; // Optional
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	//pipelineInfo.flags = VK_PIPELINE_CREATE_DESCRIPTOR_BUFFER_BIT_EXT;
	pipelineInfo.layout = m_layout;
	pipelineInfo.renderPass = VulkanContext::getRenderPass()->getVkRenderPass();
	pipelineInfo.subpass = 0;

	VK_CHECK(vkCreateGraphicsPipelines(VulkanContext::getDevice()->getVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline));

	for (auto shaderModule : shaderModules)
	{
		vkDestroyShaderModule(VulkanContext::getDevice()->getVkDevice(), shaderModule, nullptr);
	}
}

void Pipeline::createComputePipeline(const PipelineCreateInfo& pipelineCreateInfo, DescriptorSetLayout* descriptorSetLayout)
{
	// 1. 加载计算着色器
	std::vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos;
	std::vector<VkShaderModule> shaderModules;
	for (auto shader : pipelineCreateInfo.m_shaders)
	{
		auto shaderCode = readFile(shader->getFileName());

		VkShaderModule shaderModule = createShaderModule(shaderCode);

		VkPipelineShaderStageCreateInfo shaderStageInfo{};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = ShaderTypeToVk(shader->getType());
		shaderStageInfo.module = shaderModule;
		shaderStageInfo.pName = shader->getFuncName().data();

		shaderModules.push_back(shaderModule);
		shaderStageCreateInfos.push_back(shaderStageInfo);
	}

	std::vector<VkDescriptorSetLayout> setLayouts;
	setLayouts.push_back(m_descriptorSetLayout->getVkDescriptorSetLayout());

	// 4. 创建管线布局
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(uint32_t) + sizeof(float); // gridSize + cellSize

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = setLayouts.size();
	pipelineLayoutInfo.pSetLayouts = setLayouts.data();
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

	VK_CHECK(vkCreatePipelineLayout(VulkanContext::getDevice()->getVkDevice(), &pipelineLayoutInfo, nullptr, &m_layout))

	// 5. 创建计算管线
	VkComputePipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	pipelineInfo.stage = shaderStageCreateInfos.front();
	pipelineInfo.layout = m_layout;

	VK_CHECK(vkCreateComputePipelines(VulkanContext::getDevice()->getVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline));

	// 清理着色器模块
	for (auto shaderModule : shaderModules)
	{
		vkDestroyShaderModule(VulkanContext::getDevice()->getVkDevice(), shaderModule, nullptr);
	}
}
