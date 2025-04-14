#pragma once

#include <vector>

#include "VulkanContext/Exports.h"

#include "Core/RenderingContextDriver/RenderingContextDriver.h"

class Instance;
class DebugUtilsMessenger;
class PhysicalDevice;
class Surface;
class LogicalDevice;
class RenderPass;
class SwapChain;
class CommandPool;
class DescriptorPool;

class VULKANCONTEXT_API VulkanContext : public RenderingContextDriver
{
public:
	VulkanContext();
	~VulkanContext() override;

	void initialize() override final;

	void destroy();

	void wait() override final;

	void resizeSurface() override final;

	CommandBufferID createCommandBuffer() override final;
	void resetCommandBuffer(CommandBufferID commandBuffer) override final;
	void beginCommandBuffer(CommandBufferID commandBuffer) override final;
	void endCommandBuffer(CommandBufferID commandBuffer) override final;

	BufferID createUniformBuffer() override final;
	BufferID createVertexBuffer(const std::vector<Vertex>& vertices) override final;
	BufferID createIndexBuffer(const std::vector<uint32_t>& indices) override final;
	void freeBuffer(BufferID buffer) override final;
	void updateUniformBuffer(BufferID uniformBuffer, void* data, uint32_t size) override final;

	TextureID createTexture(const uint32_t width, const uint32_t height, const uint32_t channels, const unsigned char* data) override final;
	void freeTexture(TextureID buffer) override final;

	FenceID createFence() override final;
	void waitFence(FenceID p_fence) override final;
	void freeFence(FenceID p_fence) override final;

	SemaphoreID createSemaphore() override final;
	void freeSemaphore(SemaphoreID p_semaphore) override final;

	uint32_t getNextImageIndex(SemaphoreID semaphore) override final;

	SurfaceID getSurfaceID() override final;
	Extent2D getSurfaceExtent(SurfaceID) override final;

	PipelineID createPipeline() override final;
	PipelineID createSkyboxPipeline() override final;
	void freePipeline(PipelineID pipeline) override final;

	RenderPassID getRenderPassID() override final;
	void beginRenderPass(CommandBufferID, FramebufferID) override final;
	void endRenderPass(CommandBufferID) override final;

	SwapChainID getSwapChainID() override final;
	FramebufferID getFramebuffer(SwapChainID, uint32_t) override final;

	UniformSetID createUniformSet(PipelineID pipeline) override final;
	void updateUniformSet(UniformSetID uniformSet, const std::vector<BoundUniform>& boundUniforms);
	void freeUniformSet(UniformSetID uniformSet) override final;

	void queueSubmit(const std::vector<CommandBufferID>&, const std::vector<SemaphoreID>& waitSemaphore,
		const std::vector<SemaphoreID>& signalSemaphore, const FenceID& fence) override final;

	void queuePresent(const std::vector<SemaphoreID>& waitSemaphore, const uint32_t) override final;

	void cmdBindPipeline(CommandBufferID p_cmd_buffer, PipelineID p_pipeline) override final;
	void cmdDraw(
		CommandBufferID p_cmd_buffer,
		uint32_t vertexCount,
		uint32_t instanceCount,
		uint32_t firstVertex,
		uint32_t firstInstanc);

	void cmdDrawIndexed(
		CommandBufferID p_cmd_buffer,
		uint32_t indexCount,
		uint32_t instanceCount,
		uint32_t firstIndex,
		int32_t  vertexOffset,
		uint32_t firstInstance);

	void cmdSetScissor(CommandBufferID p_cmd_buffer, const Rect2D& scissor) override final;

	void cmdSetViewport(CommandBufferID p_cmd_buffer, const Viewport& viewport) override final;

	void cmdBindVertexBuffer(CommandBufferID p_cmd_buffer, BufferID buffer) override final;

	void cmdBindIndexBuffer(CommandBufferID p_cmd_buffer, BufferID buffer) override final;

	void cmdBindDescriptorSets(CommandBufferID p_cmd_buffer, PipelineID pipeline, UniformSetID uniformSet) override final;

	void cmdPushConstants(CommandBufferID p_cmd_buffer, PipelineID pipeline, int32_t size, void* data)override final;

	void cmdSetPolygonMode(CommandBufferID p_cmd_buffer, FillMode fillMode) override final;

	TextureID createKTXTexture(const std::string_view&) override final;

	static const std::vector<const char*>& getInstanceExtensions();
	static const std::vector<const char*>& getDeviceExtensions();
	static const std::vector<const char*>& getInstanceLayers();

	static Instance* getVulkanInstance()
	{
		return m_instance;
	}

	static PhysicalDevice* getPhysicalDevice()
	{
		return m_physicalDevice;
	}

	static Surface* getSurface()
	{
		return m_surface;
	}

	static LogicalDevice* getDevice()
	{
		return m_device;
	}

	static RenderPass* getRenderPass()
	{
		return m_renderPass;
	}

	static SwapChain* getSwapChain()
	{
		return m_swapChain;
	}

	static CommandPool* getCommandPool()
	{
		return m_commandPool;
	}

private:
	static std::vector<const char*> getRequiredInstanceExtensions();

private:
	static std::vector<const char*> m_instanceExtensions;
	static std::vector<const char*> m_instanceLayers;
	static std::vector<const char*> m_deviceExtensions;

	static Instance* m_instance;
	static DebugUtilsMessenger* m_debugUtilsMessenger;
	static PhysicalDevice* m_physicalDevice;
	static Surface* m_surface;
	static LogicalDevice* m_device;
	static RenderPass* m_renderPass;
	static SwapChain* m_swapChain;
	static CommandPool* m_commandPool;
	DescriptorPool* m_descriptorPool;
};