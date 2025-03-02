#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

#include "RenderingContextDriver/Frame.h"
#include "RenderingContextDriver/RenderingContextDriver.h"
#include "RenderingContextDriver/PipelineManager.h"

Frame::Frame()
	:m_imageIndex(0)
{
	m_inFlightFence = RenderingContextDriver::instance()->createFence();
	m_renderFinishedSemaphore = RenderingContextDriver::instance()->createSemaphore();
	m_imageAvailableSemaphore = RenderingContextDriver::instance()->createSemaphore();

	m_commandBuffer = RenderingContextDriver::instance()->createCommandBuffer();

	m_uniformSet = RenderingContextDriver::instance()->createUniformSet(PipelineManager::instance()->getPipeline(PipelineType::Model));
}

Frame::~Frame()
{
	RenderingContextDriver::instance()->freeFence(m_inFlightFence);
	RenderingContextDriver::instance()->freeSemaphore(m_renderFinishedSemaphore);
	RenderingContextDriver::instance()->freeSemaphore(m_imageAvailableSemaphore);

	RenderingContextDriver::instance()->freeUniformSet(m_uniformSet);
}

CommandBufferID Frame::getCommandBuffer() const
{
	return m_commandBuffer;
}

void Frame::begin()
{
	RenderingContextDriver::instance()->waitFence(m_inFlightFence);

	RenderingContextDriver::instance()->resetCommandBuffer(m_commandBuffer);

	RenderingContextDriver::instance()->beginCommandBuffer(m_commandBuffer);

	m_imageIndex = RenderingContextDriver::instance()->getNextImageIndex(m_imageAvailableSemaphore);
	SwapChainID swapChain = RenderingContextDriver::instance()->getSwapChainID();
	FramebufferID framebuffer = RenderingContextDriver::instance()->getFramebuffer(swapChain, m_imageIndex);

	RenderingContextDriver::instance()->beginRenderPass(m_commandBuffer, framebuffer);

	updateUniformBuffer();
}

void Frame::end()
{
	RenderingContextDriver::instance()->endRenderPass(m_commandBuffer);

	RenderingContextDriver::instance()->endCommandBuffer(m_commandBuffer);

	RenderingContextDriver::instance()->queueSubmit({ m_commandBuffer }, { m_imageAvailableSemaphore }, { m_renderFinishedSemaphore }, m_inFlightFence);

	RenderingContextDriver::instance()->queuePresent({ m_renderFinishedSemaphore },m_imageIndex);
}

void Frame::updateUniformBuffer()
{
	const Extent2D& surfaceExtent = RenderingContextDriver::instance()->getSurfaceExtent(SurfaceID());

	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo{};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	ubo.proj = glm::perspective(glm::radians(45.0f), surfaceExtent.width / (float)surfaceExtent.height, 0.1f, 10.0f);

	ubo.proj[1][1] *= -1;

	RenderingContextDriver::instance()->undateUniformSet(m_uniformSet, ubo);

	RenderingContextDriver::instance()->cmdBindDescriptorSets(m_commandBuffer, PipelineManager::instance()->getPipeline(PipelineType::Model), m_uniformSet);
}
