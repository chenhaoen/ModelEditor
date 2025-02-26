#pragma once

#include <vector>

#include "VulkanContext/Exports.h"

#include "RenderingContextDriver/RenderingContextDriver.h"

class Instance;
class DebugUtilsMessenger;
class PhysicalDevice;
class Surface;
class LogicalDevice;
class RenderPass;
class SwapChain;
class CommandPool;

class VULKANCONTEXT_API VulkanContext : public RenderingContextDriver
{
public:
	VulkanContext();
	~VulkanContext() override;

	void initialize() override final;

	void destroy();

	void wait() override final;

	void draw() override final;

	void resizeSurface() override final;

	CommandBufferID createCommandBuffer() override final;
	void resetCommandBuffer(CommandBufferID commandBuffer) override final;
	void beginCommandBuffer(CommandBufferID commandBuffer) override final;
	void endCommandBuffer(CommandBufferID commandBuffer) override final;

	FenceID createFence() override final;
	void waitFence(FenceID p_fence) override final;
	void freeFence(FenceID p_fence) override final;

	SemaphoreID createSemaphore() override final;
	void freeSemaphore(SemaphoreID p_semaphore) override final;

	uint32_t getNextImageIndex(SemaphoreID semaphore) override final;

	SurfaceID getSurfaceID() override final;
	Extent2D getSurfaceExtent(SurfaceID) override final;

	RenderPassID getRenderPassID() override final;
	void beginRenderPass(CommandBufferID, FramebufferID) override final;
	void endRenderPass(CommandBufferID) override final;

	SwapChainID getSwapChainID() override final;
	FramebufferID getFramebuffer(SwapChainID, uint32_t) override final;

	void queueSubmit(const std::vector<CommandBufferID>&, const std::vector<SemaphoreID>& waitSemaphore,
		const std::vector<SemaphoreID>& signalSemaphore, const FenceID& fence) override final;

	void queuePresent(const std::vector<SemaphoreID>& waitSemaphore, const uint32_t) override final;

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
};