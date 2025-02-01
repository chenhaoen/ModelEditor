#include <stdexcept>
#include <array>

#include "VulkanContext/Frame.h"
#include "VulkanContext/VulkanContext.h"
#include "VulkanContext/LogicalDevice.h"
#include "VulkanContext/CommandPool.h"
#include "VulkanContext/SwapChain.h"
#include "VulkanContext/RenderPass.h"
#include "VulkanContext/Surface.h"
#include "VulkanContext/Utils.h"
#include "VulkanContext/Pipeline.h"
#include "VulkanContext/DescriptorSetLayout.h"

Frame::Frame(uint32_t index)
	: m_index(index)
	, m_imageIndex(0)
	, m_clearValue{ 0.0f, 0.0f, 0.0f, 1.0f }
{
	m_commandBuffer = VulkanContext::instance()->getCommandPool()->createCommands();

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	VK_CHECK(vkCreateSemaphore(VulkanContext::instance()->getDevice()->getVkDevice(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphore));
	VK_CHECK(vkCreateSemaphore(VulkanContext::instance()->getDevice()->getVkDevice(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphore));
	VK_CHECK(vkCreateFence(VulkanContext::instance()->getDevice()->getVkDevice(), &fenceInfo, nullptr, &m_inFlightFence));
}

Frame::~Frame()
{
	vkDestroySemaphore(VulkanContext::instance()->getDevice()->getVkDevice(), m_imageAvailableSemaphore, nullptr);
	vkDestroySemaphore(VulkanContext::instance()->getDevice()->getVkDevice(), m_renderFinishedSemaphore, nullptr);
	vkDestroyFence(VulkanContext::instance()->getDevice()->getVkDevice(), m_inFlightFence, nullptr);
}

VkCommandBuffer Frame::getCommandBuffer() const
{
	return m_commandBuffer;
}

uint32_t Frame::getIndex() const
{
	return m_index;
}

void Frame::begin()
{
	vkWaitForFences(VulkanContext::instance()->getDevice()->getVkDevice(), 1, &m_inFlightFence, VK_TRUE, UINT64_MAX);

	vkResetFences(VulkanContext::instance()->getDevice()->getVkDevice(), 1, &m_inFlightFence);

	m_imageIndex = VulkanContext::instance()->getSwapChain()->getNextImageIndex(m_imageAvailableSemaphore);

	vkResetCommandBuffer(m_commandBuffer, 0);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;                  // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional

	VK_CHECK(vkBeginCommandBuffer(m_commandBuffer, &beginInfo));

	VkExtent2D swapChainExtent = VulkanContext::instance()->getSurface()->getExtent();

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = VulkanContext::instance()->getRenderPass()->getVkRenderPass();
	renderPassInfo.framebuffer = VulkanContext::instance()->getSwapChain()->getFrameBuffer(m_imageIndex);

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0] = m_clearValue;
	clearValues[1] = { 1.0f, 0 };
	renderPassInfo.clearValueCount = clearValues.size();
	renderPassInfo.pClearValues = clearValues.data();

	renderPassInfo.renderArea.extent = swapChainExtent;

	vkCmdBeginRenderPass(m_commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	//static bool a = false;
	//static Pipeline* pipeline = nullptr;
	//static DescriptorSetLayout* descriptorSetLayout = nullptr;
	//if (!a)
	//{
	//	descriptorSetLayout = new DescriptorSetLayout();
	//	pipeline = new Pipeline(descriptorSetLayout);
	//	a = true;
	//}
	//
	//vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getVkPipeline());
	//
	//VkViewport viewport{};
	//viewport.x = 0.0f;
	//viewport.y = 0.0f;
	//viewport.width = (float)VulkanContext::instance()->getSurface()->getExtent().width;
	//viewport.height = (float)VulkanContext::instance()->getSurface()->getExtent().height;
	//viewport.minDepth = 0.0f;
	//viewport.maxDepth = 1.0f;
	//vkCmdSetViewport(m_commandBuffer, 0, 1, &viewport);
	//
	//VkRect2D scissor{};
	//scissor.offset = { 0, 0 };
	//scissor.extent = VulkanContext::instance()->getSurface()->getExtent();
	//vkCmdSetScissor(m_commandBuffer, 0, 1, &scissor);
	//
	//vkCmdDraw(m_commandBuffer, 3, 1, 0, 0);
}

void Frame::end()
{
	vkCmdEndRenderPass(m_commandBuffer);

	VK_CHECK(vkEndCommandBuffer(m_commandBuffer));

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_commandBuffer;

	VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphore };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	VK_CHECK(vkQueueSubmit(VulkanContext::instance()->getDevice()->getGraphicsQueue(), 1, &submitInfo, m_inFlightFence));

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	VkSwapchainKHR swapChains[] = { VulkanContext::instance()->getSwapChain()->getVkSwapChain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &m_imageIndex;
	presentInfo.pResults = nullptr; // Optional

	vkQueuePresentKHR(VulkanContext::instance()->getDevice()->getPresentQueue(), &presentInfo);
}

void Frame::setClearValue(VkClearValue clearValue)
{
	m_clearValue = clearValue;
}

VkClearValue Frame::getClearVaule() const
{
	return m_clearValue;
}

