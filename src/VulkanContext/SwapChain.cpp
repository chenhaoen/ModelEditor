#include <algorithm>
#include <stdexcept>
#include <limits>
#include <array>

#include "VulkanContext/SwapChain.h"
#include "VulkanContext/Surface.h"
#include "VulkanContext/VulkanContext.h"
#include "VulkanContext/RenderPass.h"
#include "VulkanContext/LogicalDevice.h"
#include "VulkanContext/PhysicalDevice.h"
#include "VulkanContext/Utils.h"
#include "VulkanContext/Image.h"

bool hasStencilComponent(VkFormat format)
{
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

SwapChain::SwapChain()
{
	create();
	createImageViews();
	createFrameBuffers();
	//createDepthResources();
}

SwapChain::~SwapChain()
{
	cleanup();
}

void SwapChain::createFrameBuffers()
{
	m_frameBuffers.resize(m_imageViews.size());
	for (size_t i = 0; i < m_frameBuffers.size(); i++)
	{
		std::array<VkImageView, 1> attachments = {
			m_imageViews[i] };

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = VulkanContext::instance()->getRenderPass()->getVkRenderPass();
		framebufferInfo.attachmentCount = attachments.size();
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = VulkanContext::instance()->getSurface()->getExtent().width;
		framebufferInfo.height = VulkanContext::instance()->getSurface()->getExtent().height;
		framebufferInfo.layers = 1;

		VK_CHECK(vkCreateFramebuffer(VulkanContext::instance()->getDevice()->getVkDevice(), &framebufferInfo, nullptr, &m_frameBuffers[i]));
	}
}

void SwapChain::createDepthResources()
{
	//VkFormat depthFormat = findDepthFormat();
	//
	//createImage(m_extent.width, m_extent.height, depthFormat,
	//            VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
	//            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_depthImage, m_depthImageMemory);
	//m_depthImageView = createImageView(m_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}

void SwapChain::create()
{
	VkSurfaceFormatKHR surfaceFormat = VulkanContext::instance()->getSurface()->getSurfaceFormat();
	uint32_t imageCount = VulkanContext::instance()->getSurface()->getImageCount();

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = VulkanContext::instance()->getSurface()->getVkSurface();
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = VulkanContext::instance()->getSurface()->getImageFormat();
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = VulkanContext::instance()->getSurface()->getExtent();
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	const uint32_t graphicsFamilyIndex = VulkanContext::instance()->getPhysicalDevice()->getGraphicsFamilyIndex();
	const uint32_t presentFamilyIndex = VulkanContext::instance()->getPhysicalDevice()->getPresentFamilyIndex(VulkanContext::instance()->getSurface());
	uint32_t queueFamilyIndices[] = { graphicsFamilyIndex, presentFamilyIndex };

	if (graphicsFamilyIndex != presentFamilyIndex)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;     // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	createInfo.preTransform = VulkanContext::instance()->getSurface()->getPreTranform();
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = VulkanContext::instance()->getSurface()->getPresentMode();
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	VK_CHECK(vkCreateSwapchainKHR(VulkanContext::instance()->getDevice()->getVkDevice(), &createInfo, nullptr, &m_vkSwapChain));

	vkGetSwapchainImagesKHR(VulkanContext::instance()->getDevice()->getVkDevice(), m_vkSwapChain, &imageCount, nullptr);
	m_images.resize(imageCount);
	vkGetSwapchainImagesKHR(VulkanContext::instance()->getDevice()->getVkDevice(), m_vkSwapChain, &imageCount, m_images.data());
}

void SwapChain::createImageViews()
{
	m_imageViews.resize(m_images.size());
	for (size_t i = 0; i < m_imageViews.size(); i++)
	{
		m_imageViews[i] = createImageView(m_images[i], VulkanContext::instance()->getSurface()->getImageFormat(), VK_IMAGE_ASPECT_COLOR_BIT);
	}
}

void SwapChain::cleanup()
{
	for (auto imageView : m_imageViews)
	{
		vkDestroyImageView(VulkanContext::instance()->getDevice()->getVkDevice(), imageView, nullptr);
	}

	for (auto framebuffer : m_frameBuffers)
	{
		vkDestroyFramebuffer(VulkanContext::instance()->getDevice()->getVkDevice(), framebuffer, nullptr);
	}

	vkDestroySwapchainKHR(VulkanContext::instance()->getDevice()->getVkDevice(), m_vkSwapChain, nullptr);
	//
	//vkDestroyImage(Context::instance()->getDevice()->getVkDevice(), m_depthImage, nullptr);
	//vkFreeMemory(Context::instance()->getDevice()->getVkDevice(), m_depthImageMemory, nullptr);
	//vkDestroyImageView(Context::instance()->getDevice()->getVkDevice(), m_depthImageView, nullptr);
}

//void SwapChain::setRenderPass(RenderPass *renderPass)
//{
//   // m_renderPass = renderPass;
//}


VkSwapchainKHR SwapChain::getVkSwapChain() const
{
	return m_vkSwapChain;
}

VkFramebuffer SwapChain::getFrameBuffer(const int index)
{
	return m_frameBuffers[index];
}

uint32_t SwapChain::getNextImageIndex(VkSemaphore semaphore)
{
	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(VulkanContext::instance()->getDevice()->getVkDevice(),
		m_vkSwapChain, UINT64_MAX, semaphore, VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		return imageIndex;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	return imageIndex;
}
