#include "VulkanContext/utils.h"
#include "VulkanContext/VulkanContext.h"
#include "VulkanContext/LogicalDevice.h"
#include "VulkanContext/PhysicalDevice.h"
#include "VulkanContext/CommandPool.h"

#include "Core/RenderingContextDriver/Commons.h"

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

void createVKBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VK_CHECK(vkCreateBuffer(VulkanContext::getDevice()->getVkDevice(), &bufferInfo, nullptr, &buffer));

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(VulkanContext::getDevice()->getVkDevice(), buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = VulkanContext::getPhysicalDevice()->findMemoryType(memRequirements.memoryTypeBits, properties);

	VK_CHECK(vkAllocateMemory(VulkanContext::getDevice()->getVkDevice(), &allocInfo, nullptr, &bufferMemory));

	vkBindBufferMemory(VulkanContext::getDevice()->getVkDevice(), buffer, bufferMemory, 0);
}

void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	VkCommandBuffer commandBuffer = VulkanContext::getCommandPool()->beginSingleTimeCommands();

	VkBufferCopy copyRegion{};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	VulkanContext::getCommandPool()->endSingleTimeCommands(commandBuffer);
}

std::vector<char> readFile(const std::string_view& filename)
{
	std::ifstream file(filename.data(), std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}

VkShaderModule createShaderModule(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	VK_CHECK(vkCreateShaderModule(VulkanContext::getDevice()->getVkDevice(), &createInfo, nullptr, &shaderModule));
	return shaderModule;
}

VkShaderStageFlagBits ShaderTypeToVk(ShaderType shaderType)
{
	switch (shaderType)
	{
	case ShaderType::Vertex:
		return VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
		break;
	case ShaderType::Fragment:
		return VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
		break;
	case ShaderType::Compute:
		return VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT;
		break;
	default:
		break;
	}

	return VkShaderStageFlagBits::VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
}

VkDescriptorType UniformTypeToVK(UniformType uniformType)
{
	switch (uniformType)
	{
	case UniformType::UNIFORM_TYPE_SAMPLER:
		break;
	case UniformType::UNIFORM_TYPE_SAMPLER_WITH_TEXTURE:
		return VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		break;
	case UniformType::UNIFORM_TYPE_TEXTURE:
		break;
	case UniformType::UNIFORM_TYPE_IMAGE:
		break;
	case UniformType::UNIFORM_TYPE_TEXTURE_BUFFER:
		break;
	case UniformType::UNIFORM_TYPE_SAMPLER_WITH_TEXTURE_BUFFER:
		break;
	case UniformType::UNIFORM_TYPE_IMAGE_BUFFER:
		break;
	case UniformType::UNIFORM_TYPE_UNIFORM_BUFFER:
		return VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		break;
	case UniformType::UNIFORM_TYPE_STORAGE_BUFFER:
		return VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		break;
	case UniformType::UNIFORM_TYPE_INPUT_ATTACHMENT:
		break;
	case UniformType::UNIFORM_TYPE_MAX:
		break;
	default:
		break;
	}

	return VkDescriptorType::VK_DESCRIPTOR_TYPE_MAX_ENUM;
}

void RasterizationStateToVK(VkPipelineRasterizationStateCreateInfo& createInfo, const PipelineRasterizationState& rasterizationState)
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	createInfo.depthClampEnable = rasterizationState.enable_depth_clamp;
	createInfo.rasterizerDiscardEnable = rasterizationState.discard_primitives;
	createInfo.polygonMode = rasterizationState.wireframe ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL;
	createInfo.lineWidth = rasterizationState.line_width;

	switch (rasterizationState.cull_mode)
	{
	case PolygonCullMode::POLYGON_CULL_DISABLED:
		createInfo.cullMode = VK_CULL_MODE_NONE;
		break;
	case PolygonCullMode::POLYGON_CULL_FRONT:
		createInfo.cullMode = VK_CULL_MODE_FRONT_BIT;
		break;
	case PolygonCullMode::POLYGON_CULL_BACK:
		createInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		break;
	case PolygonCullMode::POLYGON_CULL_MAX:
		createInfo.cullMode = VK_CULL_MODE_FRONT_AND_BACK;
		break;
	default:
		break;
	}

	switch (rasterizationState.front_face)
	{
	case PolygonFrontFace::POLYGON_FRONT_FACE_CLOCKWISE:
		createInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		break;
	case PolygonFrontFace::POLYGON_FRONT_FACE_COUNTER_CLOCKWISE:
		createInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		break;
	default:
		break;
	}

	createInfo.depthBiasEnable = rasterizationState.depth_bias_enabled;
	createInfo.depthBiasConstantFactor = rasterizationState.depth_bias_constant_factor; // Optional
	createInfo.depthBiasClamp = rasterizationState.depth_bias_clamp;          // Optional
	createInfo.depthBiasSlopeFactor = rasterizationState.depth_bias_slope_factor;    // Optional
}

void InputAssemblyStateToVK(VkPipelineInputAssemblyStateCreateInfo& createInfo, const RenderPrimitive renderPrimitive)
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	createInfo.topology = RD_TO_VK_PRIMITIVE[static_cast<int>(renderPrimitive)];
	createInfo.primitiveRestartEnable = (renderPrimitive == RenderPrimitive::RENDER_PRIMITIVE_TRIANGLE_STRIPS_WITH_RESTART_INDEX);
}

void MultisampleStateToVK(VkPipelineMultisampleStateCreateInfo& createInfo, const PipelineMultisampleState& multisampleState)
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	createInfo.sampleShadingEnable = multisampleState.enable_sample_shading;
	createInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	createInfo.minSampleShading = multisampleState.min_sample_shading;          // Optional
	createInfo.pSampleMask = multisampleState.sample_mask.data();            // Optional
	createInfo.alphaToCoverageEnable = multisampleState.enable_alpha_to_coverage; // Optional
	createInfo.alphaToOneEnable = multisampleState.enable_alpha_to_one;      // Optional
}

void VertexInputStateToVK(std::vector<VkVertexInputBindingDescription>& bindings,
	std::vector<VkVertexInputAttributeDescription>& attributes, const VertexInputState& vertexInputeState)
{
	for (const auto& binding : vertexInputeState.bindings)
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = binding.binding;
		bindingDescription.stride = binding.stride;
		bindingDescription.inputRate = (binding.frequency == VertexFrequency::VERTEX_FREQUENCY_VERTEX) ? VK_VERTEX_INPUT_RATE_VERTEX : VK_VERTEX_INPUT_RATE_INSTANCE;

		for (const auto& attribute : binding.vertexAttributes)
		{
			VkVertexInputAttributeDescription attributeDescription{};
			attributeDescription.binding = binding.binding;
			attributeDescription.location = attribute.location;
			attributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescription.offset = attribute.offset;
			attributes.push_back(attributeDescription);
		}

		bindings.push_back(bindingDescription);
	}
}