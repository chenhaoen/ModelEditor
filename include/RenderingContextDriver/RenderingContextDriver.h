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

	virtual void wait() = 0;

	virtual void resizeSurface() = 0;

	virtual CommandBufferID createCommandBuffer() = 0;
	virtual void resetCommandBuffer(CommandBufferID commandBuffer) = 0;
	virtual void beginCommandBuffer(CommandBufferID commandBuffer) = 0;
	virtual void endCommandBuffer(CommandBufferID commandBuffer) = 0;

	virtual BufferID createVertexBuffer(const std::vector<Vertex>& vertices) = 0;
	virtual BufferID createIndexBuffer(const std::vector<uint32_t>& indices) = 0;
	virtual void freeBuffer(BufferID buffer) = 0;

	virtual TextureID createTexture(const uint32_t width, const uint32_t height, const uint32_t channels, const unsigned char* data) = 0;
	virtual void freeTexture(TextureID buffer) = 0;

	virtual FenceID createFence() = 0;
	virtual void waitFence(FenceID p_fence) = 0;
	virtual void freeFence(FenceID p_fence) = 0;

	virtual SemaphoreID createSemaphore() = 0;
	virtual void freeSemaphore(SemaphoreID p_semaphore) = 0;

	virtual uint32_t getNextImageIndex(SemaphoreID semaphore) = 0;

	virtual SurfaceID getSurfaceID() = 0;
	virtual Extent2D getSurfaceExtent(SurfaceID) = 0;

	virtual PipelineID createPipeline() = 0;
	virtual void freePipeline(PipelineID pipeline) = 0;

	virtual RenderPassID getRenderPassID() = 0;
	virtual void beginRenderPass(CommandBufferID, FramebufferID) = 0;
	virtual void endRenderPass(CommandBufferID) = 0;

	virtual SwapChainID getSwapChainID() = 0;
	virtual FramebufferID getFramebuffer(SwapChainID, uint32_t) = 0;

	virtual UniformSetID createUniformSet(PipelineID pipeline,const std::vector<BoundUniform>& boundUniforms) = 0;
	virtual void freeUniformSet(UniformSetID uniformSet) = 0;
	virtual void undateUniformSet(UniformSetID uniformSet, const UniformBufferObject& ubo) = 0;

	virtual void queueSubmit(const std::vector<CommandBufferID>&, const std::vector<SemaphoreID>& waitSemaphore,
		const std::vector<SemaphoreID>& signalSemaphore, const FenceID& fence) = 0;

	virtual void queuePresent(const std::vector<SemaphoreID>& waitSemaphore, const uint32_t) = 0;

	virtual void cmdDraw(
		CommandBufferID p_cmd_buffer,
		uint32_t vertexCount,
		uint32_t instanceCount,
		uint32_t firstVertex,
		uint32_t firstInstanc) = 0;

	virtual void cmdDrawIndexed(
		CommandBufferID p_cmd_buffer,
		uint32_t indexCount,
		uint32_t instanceCount,
		uint32_t firstIndex,
		int32_t  vertexOffset,
		uint32_t firstInstance) = 0;

	virtual void cmdBindPipeline(CommandBufferID p_cmd_buffer, PipelineID p_pipeline) = 0;

	virtual void cmdSetScissor(CommandBufferID p_cmd_buffer, const Rect2D& scissor) = 0;

	virtual void cmdSetViewport(CommandBufferID p_cmd_buffer, const Viewport& viewport) = 0;

	virtual void cmdBindVertexBuffer(CommandBufferID p_cmd_buffer, BufferID buffer) = 0;

	virtual void cmdBindIndexBuffer(CommandBufferID p_cmd_buffer, BufferID buffer) = 0;

	virtual void cmdBindDescriptorSets(CommandBufferID p_cmd_buffer, PipelineID pipeline, UniformSetID uniformSet) = 0;
protected:
	RenderingContextDriver() = default;
	virtual ~RenderingContextDriver() = 0;

protected:
	InitInfo m_initInfo;

};