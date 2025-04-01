#include <iostream>
#include <chrono>

#include <vulkan/vulkan.h>

#ifdef _WIN64
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#endif // _Win64

#include "VulkanContext/VulkanContext.h"
#include "VulkanContext/Instance.h"
#include "VulkanContext/DebugUtilsMessenger.h"
#include "VulkanContext/LogicalDevice.h"
#include "VulkanContext/PhysicalDevice.h"
#include "VulkanContext/Surface.h"
#include "VulkanContext/RenderPass.h"
#include "VulkanContext/SwapChain.h"
#include "VulkanContext/CommandPool.h"
#include "VulkanContext/Pipeline.h"
#include "VulkanContext/DescriptorSetLayout.h"
#include "VulkanContext/DescriptorPool.h"
#include "VulkanContext/Utils.h"
#include "VulkanContext/Image.h"

#include "Core/Mesh.h"

Instance* VulkanContext::m_instance = nullptr;
DebugUtilsMessenger* VulkanContext::m_debugUtilsMessenger = nullptr;
PhysicalDevice* VulkanContext::m_physicalDevice = nullptr;
Surface* VulkanContext::m_surface = nullptr;
LogicalDevice* VulkanContext::m_device = nullptr;
RenderPass* VulkanContext::m_renderPass = nullptr;
SwapChain* VulkanContext::m_swapChain = nullptr;
CommandPool* VulkanContext::m_commandPool = nullptr;
std::vector<const char*> VulkanContext::m_instanceExtensions;
std::vector<const char*> VulkanContext::m_instanceLayers;
std::vector<const char*> VulkanContext::m_deviceExtensions;

VulkanContext::VulkanContext()
{

}

VulkanContext::~VulkanContext()
{
	destroy();
}

std::vector<const char*> VulkanContext::getRequiredInstanceExtensions()
{
	std::vector<const char*> result;

#ifdef _WIN64
	result.emplace_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif // _WIN64

	result.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);

	return result;
}

void VulkanContext::initialize()
{
#ifndef NDEBUG
	const auto start{ std::chrono::steady_clock::now() };
#endif // !NDEBUG

	const std::vector<const char*>& windowInstanceExtensions = getRequiredInstanceExtensions();
	//m_instanceExtensions.emplace_back(VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME);
	m_instanceExtensions.insert(m_instanceExtensions.end(), windowInstanceExtensions.begin(), windowInstanceExtensions.end());
	m_deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	m_deviceExtensions.emplace_back(VK_EXT_EXTENDED_DYNAMIC_STATE_3_EXTENSION_NAME);

	if (m_initInfo.m_debug)
	{
		m_instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		m_instanceLayers.push_back("VK_LAYER_KHRONOS_validation");
	}

	m_instance = new Instance();

	if (m_initInfo.m_debug)
	{
		m_debugUtilsMessenger = new DebugUtilsMessenger();
	}

	m_physicalDevice = m_instance->getBestPhysicalDevice();
	if (m_physicalDevice == nullptr)
	{
		throw std::runtime_error("failed to find a suitable GPU!");
	}

	m_surface = new Surface(m_initInfo.m_platformWindow);

	m_device = new LogicalDevice(m_physicalDevice);

	m_renderPass = new RenderPass();

	m_swapChain = new SwapChain();

	m_commandPool = new CommandPool();

	m_descriptorPool = new DescriptorPool();

#ifndef NDEBUG
	const auto finish{ std::chrono::steady_clock::now() };
	const std::chrono::duration<double> elapsed_seconds{ finish - start };

	std::cout << "Vulkan content initialize : " << elapsed_seconds << '\n'; // C++20's chrono::duration operator<<
#endif
}

void VulkanContext::destroy()
{
	m_device->wait();

	delete m_descriptorPool;
	delete m_commandPool;
	delete m_swapChain;
	delete m_renderPass;
	delete m_device;
	delete m_surface;
	delete m_physicalDevice;
	delete m_debugUtilsMessenger;
	delete m_instance;
}

void VulkanContext::wait()
{
	m_device->wait();
}

void VulkanContext::resizeSurface()
{
	if (m_swapChain == nullptr)
	{
		return;
	}

	m_device->wait();
	m_swapChain->cleanup();
	m_swapChain->create();
	m_swapChain->createImageViews();
	m_swapChain->createDepthResources();
	m_swapChain->createFrameBuffers();
}

CommandBufferID VulkanContext::createCommandBuffer()
{
	return CommandBufferID(m_commandPool->createCommands());
}

void VulkanContext::resetCommandBuffer(CommandBufferID commandBuffer)
{
	VK_CHECK(vkResetCommandBuffer(reinterpret_cast<VkCommandBuffer>(commandBuffer.id), 0));
}

void VulkanContext::beginCommandBuffer(CommandBufferID commandBuffer)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;                  // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional

	VK_CHECK(vkBeginCommandBuffer(reinterpret_cast<VkCommandBuffer>(commandBuffer.id), &beginInfo));
}

void VulkanContext::endCommandBuffer(CommandBufferID commandBuffer)
{
	VK_CHECK(vkEndCommandBuffer(reinterpret_cast<VkCommandBuffer>(commandBuffer.id)));
}

struct BufferInfo
{
	VkBuffer buffer;
	VkDeviceMemory memory;
	void* mapped;
};

BufferID VulkanContext::createUniformBuffer()
{
	BufferInfo* bufferInfo = new BufferInfo;
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);
	createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, bufferInfo->buffer, bufferInfo->memory);

	vkMapMemory(VulkanContext::getDevice()->getVkDevice(), bufferInfo->memory, 0, bufferSize, 0, &bufferInfo->mapped);
	return BufferID(bufferInfo);
}

BufferID VulkanContext::createVertexBuffer(const std::vector<Vertex>& vertices)
{
	BufferInfo* bufferInfo = new BufferInfo{};

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(VulkanContext::getDevice()->getVkDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(VulkanContext::getDevice()->getVkDevice(), stagingBufferMemory);

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		bufferInfo->buffer, bufferInfo->memory);

	copyBuffer(stagingBuffer, bufferInfo->buffer, bufferSize);

	vkDestroyBuffer(VulkanContext::getDevice()->getVkDevice(), stagingBuffer, nullptr);
	vkFreeMemory(VulkanContext::getDevice()->getVkDevice(), stagingBufferMemory, nullptr);

	return BufferID(bufferInfo);
}

BufferID VulkanContext::createIndexBuffer(const std::vector<uint32_t>& indices)
{
	BufferInfo* bufferInfo = new BufferInfo{};

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(VulkanContext::getDevice()->getVkDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(VulkanContext::getDevice()->getVkDevice(), stagingBufferMemory);

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		bufferInfo->buffer, bufferInfo->memory);

	copyBuffer(stagingBuffer, bufferInfo->buffer, bufferSize);

	vkDestroyBuffer(VulkanContext::getDevice()->getVkDevice(), stagingBuffer, nullptr);
	vkFreeMemory(VulkanContext::getDevice()->getVkDevice(), stagingBufferMemory, nullptr);

	return BufferID(bufferInfo);
}

void VulkanContext::freeBuffer(BufferID buffer)
{
	if (!buffer)
	{
		return;
	}

	BufferInfo* bufferInfo = reinterpret_cast<BufferInfo*>(buffer.id);

	vkDestroyBuffer(VulkanContext::getDevice()->getVkDevice(), bufferInfo->buffer, nullptr);
	vkFreeMemory(VulkanContext::getDevice()->getVkDevice(), bufferInfo->memory, nullptr);

	delete bufferInfo;
}

struct TextureInfo
{
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;
};

TextureID VulkanContext::createTexture(const uint32_t width, const uint32_t height, const uint32_t channels, const unsigned char* imagedata)
{
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	TextureInfo* pTextureInfo = new TextureInfo();
	VkDeviceSize imageSize = width * height * channels;
	createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(VulkanContext::getDevice()->getVkDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, imagedata, static_cast<size_t>(imageSize));
	vkUnmapMemory(VulkanContext::getDevice()->getVkDevice(), stagingBufferMemory);

	createImage(width, height,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		pTextureInfo->textureImage,
		pTextureInfo->textureImageMemory);

	transitionImageLayout(pTextureInfo->textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	copyBufferToImage(stagingBuffer, pTextureInfo->textureImage, static_cast<uint32_t>(width), static_cast<uint32_t>(height));

	transitionImageLayout(pTextureInfo->textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(VulkanContext::getDevice()->getVkDevice(), stagingBuffer, nullptr);
	vkFreeMemory(VulkanContext::getDevice()->getVkDevice(), stagingBufferMemory, nullptr);

	pTextureInfo->textureImageView = createImageView(pTextureInfo->textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = VulkanContext::getPhysicalDevice()->properties().limits.maxSamplerAnisotropy;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	VK_CHECK(vkCreateSampler(VulkanContext::getDevice()->getVkDevice(), &samplerInfo, nullptr, &pTextureInfo->textureSampler));

	return TextureID(pTextureInfo);
}

void VulkanContext::freeTexture(TextureID buffer)
{
	if (!buffer)
	{
		return;
	}

	TextureInfo* bufferInfo = reinterpret_cast<TextureInfo*>(buffer.id);

	vkDestroySampler(VulkanContext::getDevice()->getVkDevice(), bufferInfo->textureSampler, nullptr);
	vkDestroyImageView(VulkanContext::getDevice()->getVkDevice(), bufferInfo->textureImageView, nullptr);
	vkDestroyImage(VulkanContext::getDevice()->getVkDevice(), bufferInfo->textureImage, nullptr);
	vkFreeMemory(VulkanContext::getDevice()->getVkDevice(), bufferInfo->textureImageMemory, nullptr);
}

FenceID VulkanContext::createFence()
{
	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	VkFence fence;
	VK_CHECK(vkCreateFence(VulkanContext::getDevice()->getVkDevice(), &fenceInfo, nullptr, &fence));

	return FenceID(fence);
}

void VulkanContext::waitFence(FenceID p_fence)
{
	VK_CHECK(vkWaitForFences(VulkanContext::getDevice()->getVkDevice(), 1, reinterpret_cast<VkFence*>(&p_fence.id), VK_TRUE, UINT64_MAX));

	VK_CHECK(vkResetFences(VulkanContext::getDevice()->getVkDevice(), 1, reinterpret_cast<VkFence*>(&p_fence.id)));
}

void VulkanContext::freeFence(FenceID p_fence)
{
	vkDestroyFence(VulkanContext::getDevice()->getVkDevice(), reinterpret_cast<VkFence>(p_fence.id), nullptr);
}

SemaphoreID VulkanContext::createSemaphore()
{
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkSemaphore semaphore;
	VK_CHECK(vkCreateSemaphore(VulkanContext::getDevice()->getVkDevice(), &semaphoreInfo, nullptr, &semaphore));

	return SemaphoreID(semaphore);
}

void VulkanContext::freeSemaphore(SemaphoreID p_semaphore)
{
	vkDestroySemaphore(VulkanContext::getDevice()->getVkDevice(), reinterpret_cast<VkSemaphore>(p_semaphore.id), nullptr);
}

uint32_t VulkanContext::getNextImageIndex(SemaphoreID semaphore)
{
	return m_swapChain->getNextImageIndex(reinterpret_cast<VkSemaphore>(semaphore.id));
}

SurfaceID VulkanContext::getSurfaceID()
{
	return SurfaceID(m_surface);
}

Extent2D VulkanContext::getSurfaceExtent(SurfaceID)
{
	Extent2D result;
	result.width = m_surface->getExtent().width;
	result.height = m_surface->getExtent().height;
	return result;
}

struct PipelineInfo
{
	Pipeline* pipeline;
	DescriptorSetLayout* descriptorSetLayout;
};

PipelineID VulkanContext::createPipeline()
{
	PipelineInfo* pipelineInfo = new PipelineInfo();
	pipelineInfo->descriptorSetLayout = new DescriptorSetLayout();
	pipelineInfo->pipeline = new Pipeline(pipelineInfo->descriptorSetLayout);
	return PipelineID(pipelineInfo);
}

void VulkanContext::freePipeline(PipelineID pipeline)
{
	PipelineInfo* pipelineInfo = reinterpret_cast<PipelineInfo*>(pipeline.id);

	delete pipelineInfo->descriptorSetLayout;
	delete pipelineInfo->pipeline;
	delete pipelineInfo;
}

void VulkanContext::cmdBindPipeline(CommandBufferID p_cmd_buffer, PipelineID p_pipeline)
{
	PipelineInfo* pipelineInfo = reinterpret_cast<PipelineInfo*>(p_pipeline.id);
	vkCmdBindPipeline(reinterpret_cast<VkCommandBuffer>(p_cmd_buffer.id),
		VK_PIPELINE_BIND_POINT_GRAPHICS, reinterpret_cast<Pipeline*>(pipelineInfo->pipeline)->getVkPipeline());
}

void VulkanContext::cmdDraw(CommandBufferID p_cmd_buffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstanc)
{
	vkCmdDraw(reinterpret_cast<VkCommandBuffer>(p_cmd_buffer.id),
		vertexCount, instanceCount, firstVertex, firstInstanc);
}

void VulkanContext::cmdDrawIndexed(CommandBufferID p_cmd_buffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
	vkCmdDrawIndexed(reinterpret_cast<VkCommandBuffer>(p_cmd_buffer.id),
		indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

static_assert(sizeof(Rect2D) == sizeof(VkRect2D));

void VulkanContext::cmdSetScissor(CommandBufferID p_cmd_buffer, const Rect2D& scissor)
{
	VkRect2D vkScissor;
	memcpy(&vkScissor, &scissor, sizeof(vkScissor));

	vkCmdSetScissor(reinterpret_cast<VkCommandBuffer>(p_cmd_buffer.id),
		0, 1, &vkScissor);
}

static_assert(sizeof(Viewport) == sizeof(VkViewport));

void VulkanContext::cmdSetViewport(CommandBufferID p_cmd_buffer, const Viewport& viewport)
{
	VkViewport vkViewport;
	memcpy(&vkViewport, &viewport, sizeof(VkViewport));

	vkCmdSetViewport(reinterpret_cast<VkCommandBuffer>(p_cmd_buffer.id),
		0, 1, &vkViewport);
}

void VulkanContext::cmdBindVertexBuffer(CommandBufferID p_cmd_buffer, BufferID buffer)
{
	if (!buffer)
	{
		return;
	}

	BufferInfo* bufferInfo = reinterpret_cast<BufferInfo*>(buffer.id);

	VkBuffer vertexBuffers[] = { bufferInfo->buffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(reinterpret_cast<VkCommandBuffer>(p_cmd_buffer.id), 0, 1, vertexBuffers, offsets);
}

void VulkanContext::cmdBindIndexBuffer(CommandBufferID p_cmd_buffer, BufferID buffer)
{
	if (!buffer)
	{
		return;
	}

	BufferInfo* bufferInfo = reinterpret_cast<BufferInfo*>(buffer.id);
	vkCmdBindIndexBuffer(reinterpret_cast<VkCommandBuffer>(p_cmd_buffer.id), bufferInfo->buffer, 0, VK_INDEX_TYPE_UINT32);
}

struct UniformBufferInfo
{
	VkDescriptorSet descriptorSet;
};

void VulkanContext::cmdBindDescriptorSets(CommandBufferID p_cmd_buffer, PipelineID pipeline, UniformSetID uniformSet)
{
	PipelineInfo* pipelineInfo = reinterpret_cast<PipelineInfo*>(pipeline.id);
	UniformBufferInfo* bufferInfo = reinterpret_cast<UniformBufferInfo*>(uniformSet.id);

	vkCmdBindDescriptorSets(reinterpret_cast<VkCommandBuffer>(p_cmd_buffer.id),
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		pipelineInfo->pipeline->getVkPipelineLayout(),
		0, 1,
		&bufferInfo->descriptorSet, 0, nullptr);
}

void VulkanContext::cmdPushConstants(CommandBufferID p_cmd_buffer, PipelineID pipeline, int32_t size, void* data)
{
	PipelineInfo* pipelineInfo = reinterpret_cast<PipelineInfo*>(pipeline.id);
	vkCmdPushConstants(
		reinterpret_cast<VkCommandBuffer>(p_cmd_buffer.id), // 命令缓冲区
		pipelineInfo->pipeline->getVkPipelineLayout(),
		VK_SHADER_STAGE_VERTEX_BIT, // 着色器阶段
		0, // 偏移量
		size, // 数据大小
		data // 数据
	);
}



void VulkanContext::cmdSetPolygonMode(CommandBufferID p_cmd_buffer, FillMode fillMode)
{
	// 获取函数指针（需动态加载）
	PFN_vkCmdSetPolygonModeEXT vkCmdSetPolygonModeEXT =
		(PFN_vkCmdSetPolygonModeEXT)vkGetDeviceProcAddr(m_device->getVkDevice(), "vkCmdSetPolygonModeEXT");

	switch (fillMode)
	{
	case FillMode::SOLID:
		vkCmdSetPolygonModeEXT(reinterpret_cast<VkCommandBuffer>(p_cmd_buffer.id), VK_POLYGON_MODE_FILL);
		break;
	case FillMode::WIREFRAME:
		vkCmdSetPolygonModeEXT(reinterpret_cast<VkCommandBuffer>(p_cmd_buffer.id), VK_POLYGON_MODE_LINE);
		break;
	case FillMode::POINT:
		vkCmdSetPolygonModeEXT(reinterpret_cast<VkCommandBuffer>(p_cmd_buffer.id), VK_POLYGON_MODE_POINT);
		break;
	default:
		break;
	}

}

RenderPassID VulkanContext::getRenderPassID()
{
	return RenderPassID(m_renderPass);
}

void VulkanContext::beginRenderPass(CommandBufferID commandBuffer, FramebufferID frameBuffer)
{
	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = VulkanContext::getRenderPass()->getVkRenderPass();
	renderPassInfo.framebuffer = reinterpret_cast<VkFramebuffer>(frameBuffer.id);

	std::array<VkClearValue, 2> clearValues{};

	clearValues[0] = { 135.0 / 255.0,206.0 / 255.0,250.0 / 255.0 };
	clearValues[1] = { 1.0f, 0 };
	renderPassInfo.clearValueCount = clearValues.size();
	renderPassInfo.pClearValues = clearValues.data();

	renderPassInfo.renderArea.extent = m_surface->getExtent();

	vkCmdBeginRenderPass(reinterpret_cast<VkCommandBuffer>(commandBuffer.id), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanContext::endRenderPass(CommandBufferID commandBuffer)
{
	vkCmdEndRenderPass(reinterpret_cast<VkCommandBuffer>(commandBuffer.id));
}

SwapChainID VulkanContext::getSwapChainID()
{
	return SwapChainID(m_swapChain);
}

FramebufferID VulkanContext::getFramebuffer(SwapChainID swapChain, uint32_t imageIndex)
{
	return FramebufferID(reinterpret_cast<SwapChain*>(swapChain.id)->getFrameBuffer(imageIndex));
}

UniformSetID VulkanContext::createUniformSet(PipelineID pipeline)
{
	PipelineInfo* pipelineInfo = reinterpret_cast<PipelineInfo*>(pipeline.id);
	UniformBufferInfo* bufferInfo = new UniformBufferInfo{};

	bufferInfo->descriptorSet = m_descriptorPool->AllocateDescriptorSet(pipelineInfo->descriptorSetLayout);

	return UniformSetID(bufferInfo);
}

void VulkanContext::updateUniformSet(UniformSetID uniformSet, const std::vector<BoundUniform>& boundUniforms)
{
	if (!uniformSet)
	{
		return;
	}

	UniformBufferInfo* bufferInfo = reinterpret_cast<UniformBufferInfo*>(uniformSet.id);

	std::vector< VkWriteDescriptorSet> writeDescriptorSets;

	for (const BoundUniform& boundUniform : boundUniforms)
	{
		switch (boundUniform.type)
		{
		case UNIFORM_TYPE_UNIFORM_BUFFER:
		{
			BufferInfo* UniformBufferInfo = reinterpret_cast<BufferInfo*>(boundUniform.ids[0].id);

			VkDescriptorBufferInfo descriptorBufferInfo{};
			descriptorBufferInfo.buffer = UniformBufferInfo->buffer;
			descriptorBufferInfo.offset = boundUniform.ids[1].id;
			descriptorBufferInfo.range = boundUniform.ids[2].id;

			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = bufferInfo->descriptorSet;
			descriptorWrite.dstBinding = boundUniform.binding;
			descriptorWrite.dstArrayElement = 0;

			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;

			descriptorWrite.pBufferInfo = &descriptorBufferInfo;
			writeDescriptorSets.push_back(descriptorWrite);
			break;
		}
		case UNIFORM_TYPE_SAMPLER_WITH_TEXTURE:
		{
			TextureInfo* textureInfo = reinterpret_cast<TextureInfo*>(boundUniform.ids[0].id);

			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = textureInfo->textureImageView;
			imageInfo.sampler = textureInfo->textureSampler;

			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = bufferInfo->descriptorSet;
			descriptorWrite.dstBinding = boundUniform.binding;
			descriptorWrite.dstArrayElement = 0;

			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrite.descriptorCount = 1;

			descriptorWrite.pImageInfo = &imageInfo; // Optional
			descriptorWrite.pTexelBufferView = nullptr; // Optional
			writeDescriptorSets.push_back(descriptorWrite);
		}
		default:
			break;
		}
	}

	vkUpdateDescriptorSets(VulkanContext::getDevice()->getVkDevice(), writeDescriptorSets.size(), writeDescriptorSets.data(), 0, nullptr);
}

void VulkanContext::freeUniformSet(UniformSetID uniformSet)
{
	if (!uniformSet)
	{
		return;
	}

	UniformBufferInfo* bufferInfo = reinterpret_cast<UniformBufferInfo*>(uniformSet.id);
	m_descriptorPool->freeDescriptorSets({ bufferInfo->descriptorSet });

	delete bufferInfo;
}

void VulkanContext::updateUniformBuffer(BufferID uniformBuffer, void* data, uint32_t size)
{
	if (!uniformBuffer)
	{
		return;
	}

	BufferInfo* bufferInfo = reinterpret_cast<BufferInfo*>(uniformBuffer.id);

	memcpy(bufferInfo->mapped, data, size);
}

void VulkanContext::queueSubmit(const std::vector<CommandBufferID>& inCommandBuffers, const std::vector<SemaphoreID>& inWaitSemaphore,
	const std::vector<SemaphoreID>& inSignalSemaphore, const FenceID& fence)
{
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	std::vector<VkSemaphore> waitSemaphores;
	for (SemaphoreID semaphore : inWaitSemaphore)
	{
		waitSemaphores.emplace_back(reinterpret_cast<VkSemaphore>(semaphore.id));
	}

	std::vector<VkSemaphore> signalSemaphores;
	for (SemaphoreID semaphore : inSignalSemaphore)
	{
		signalSemaphores.emplace_back(reinterpret_cast<VkSemaphore>(semaphore.id));
	}

	std::vector<VkCommandBuffer> commandBuffers;
	for (CommandBufferID commandBuffer : inCommandBuffers)
	{
		commandBuffers.emplace_back(reinterpret_cast<VkCommandBuffer>(commandBuffer.id));
	}

	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	submitInfo.waitSemaphoreCount = waitSemaphores.size();
	submitInfo.pWaitSemaphores = waitSemaphores.data();;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = commandBuffers.size();
	submitInfo.pCommandBuffers = commandBuffers.data();
	submitInfo.signalSemaphoreCount = signalSemaphores.size();
	submitInfo.pSignalSemaphores = signalSemaphores.data();

	VK_CHECK(vkQueueSubmit(VulkanContext::getDevice()->getGraphicsQueue(), 1, &submitInfo, reinterpret_cast<VkFence>(fence.id)));
}

void VulkanContext::queuePresent(const std::vector<SemaphoreID>& inWaitSemaphore, const uint32_t imageIndex)
{
	std::vector<VkSemaphore> waitSemaphores;
	for (SemaphoreID semaphore : inWaitSemaphore)
	{
		waitSemaphores.emplace_back(reinterpret_cast<VkSemaphore>(semaphore.id));
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = waitSemaphores.size();
	presentInfo.pWaitSemaphores = waitSemaphores.data();
	VkSwapchainKHR swapChains[] = { VulkanContext::getSwapChain()->getVkSwapChain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	VK_CHECK(vkQueuePresentKHR(VulkanContext::getDevice()->getPresentQueue(), &presentInfo));
}

const std::vector<const char*>& VulkanContext::getInstanceExtensions()
{
	return m_instanceExtensions;
}

const std::vector<const char*>& VulkanContext::getDeviceExtensions()
{
	return m_deviceExtensions;
}

const std::vector<const char*>& VulkanContext::getInstanceLayers()
{
	return m_instanceLayers;
}
