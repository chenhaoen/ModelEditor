#include "Core/RenderingContextDriver/RenderingContextDriver.h"

#include "Core/Frame.h"
#include "Core/Commands/CommandGroup.h"
#include "Core/PipelineManager.h"
#include "Core/Pipeline.h"

Frame::Frame()
	: m_imageIndex(0)
	, m_commandGroup(new CommandGroup())
{
	m_inFlightFence = RenderingContextDriver::instance()->createFence();
	m_renderFinishedSemaphore = RenderingContextDriver::instance()->createSemaphore();
	m_imageAvailableSemaphore = RenderingContextDriver::instance()->createSemaphore();

	m_commandBuffer = RenderingContextDriver::instance()->createCommandBuffer();
}

Frame::~Frame()
{
	RenderingContextDriver::instance()->freeFence(m_inFlightFence);
	RenderingContextDriver::instance()->freeSemaphore(m_renderFinishedSemaphore);
	RenderingContextDriver::instance()->freeSemaphore(m_imageAvailableSemaphore);
}

CommandBufferID Frame::getCommandBuffer() const
{
	return m_commandBuffer;
}

std::shared_ptr<CommandGroup> Frame::getCommandGroup() const
{
	return m_commandGroup;
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
}

void Frame::end()
{
	RenderingContextDriver::instance()->endRenderPass(m_commandBuffer);

	RenderingContextDriver::instance()->endCommandBuffer(m_commandBuffer);

	RenderingContextDriver::instance()->queueSubmit({ m_commandBuffer }, { m_imageAvailableSemaphore }, { m_renderFinishedSemaphore }, m_inFlightFence);

	RenderingContextDriver::instance()->queuePresent({ m_renderFinishedSemaphore },m_imageIndex);
}

