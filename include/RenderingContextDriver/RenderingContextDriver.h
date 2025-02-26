#pragma once

#include <vector>

#include "RenderingContextDriver/Exports.h"
#include "RenderingContextDriver/InitInfo.h"
#include "RenderingContextDriver/Commons.h"

class RENDERINGCONTEXTDRIVER_API RenderingContextDriver
{
public:
	static RenderingContextDriver* g_instance;

	static RenderingContextDriver* instance();

	static void setContext(RenderingContextDriver* driver);

	static void destroyContext();

	enum DriverType
	{
		Vulkan,
	};
	static DriverType getDriverType();

	void setInitInfo(const InitInfo& initInfo);
	const InitInfo& getInitInfo() const;

	virtual void initialize() = 0;

	virtual void draw() = 0;

	virtual void wait() = 0;

	virtual void resizeSurface() = 0;

	virtual CommandBufferID createCommandBuffer() = 0;
	virtual void resetCommandBuffer(CommandBufferID commandBuffer) = 0;
	virtual void beginCommandBuffer(CommandBufferID commandBuffer) = 0;
	virtual void endCommandBuffer(CommandBufferID commandBuffer) = 0;

	virtual FenceID createFence() = 0;
	virtual void waitFence(FenceID p_fence) = 0;
	virtual void freeFence(FenceID p_fence) = 0;

	virtual SemaphoreID createSemaphore() = 0;
	virtual void freeSemaphore(SemaphoreID p_semaphore) = 0;

	virtual uint32_t getNextImageIndex(SemaphoreID semaphore) = 0;

	virtual SurfaceID getSurfaceID() = 0;
	virtual Extent2D getSurfaceExtent(SurfaceID) = 0;

	virtual RenderPassID getRenderPassID() = 0;
	virtual void beginRenderPass(CommandBufferID, FramebufferID) = 0;
	virtual void endRenderPass(CommandBufferID) = 0;

	virtual SwapChainID getSwapChainID() = 0;
	virtual FramebufferID getFramebuffer(SwapChainID,uint32_t) = 0;

	virtual void queueSubmit(const std::vector<CommandBufferID>&, const std::vector<SemaphoreID>& waitSemaphore,
		const std::vector<SemaphoreID>& signalSemaphore, const FenceID& fence) = 0;

	virtual void queuePresent(const std::vector<SemaphoreID>& waitSemaphore, const uint32_t) = 0;
protected:
	RenderingContextDriver() = default;
	virtual ~RenderingContextDriver() = 0;

protected:
	InitInfo m_initInfo;

};