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
	:m_vkSwapChain(VK_NULL_HANDLE)
{
	create();
	createImageViews();
	createDepthResources();
	createFrameBuffers();
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
		std::array<VkImageView, 2> attachments = {
			m_imageViews[i] , m_depthImageView};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = VulkanContext::getRenderPass()->getVkRenderPass();
		framebufferInfo.attachmentCount = attachments.size();
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = VulkanContext::getSurface()->getExtent().width;
		framebufferInfo.height = VulkanContext::getSurface()->getExtent().height;
		framebufferInfo.layers = 1;

		VK_CHECK(vkCreateFramebuffer(VulkanContext::getDevice()->getVkDevice(), &framebufferInfo, nullptr, &m_frameBuffers[i]));
	}
}

void SwapChain::createDepthResources()
{
	VkFormat depthFormat = findDepthFormat();
	
	createImage(VulkanContext::getSurface()->getExtent().width, 
				VulkanContext::getSurface()->getExtent().height,
				depthFormat,
	            VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
	            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_depthImage, m_depthImageMemory);
	m_depthImageView = createImageView(m_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}

void SwapChain::create()
{
	VkSurfaceFormatKHR surfaceFormat = VulkanContext::getSurface()->getSurfaceFormat();
	uint32_t imageCount = VulkanContext::getSurface()->getImageCount();

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = VulkanContext::getSurface()->getVkSurface();
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = VulkanContext::getSurface()->getImageFormat();
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = VulkanContext::getSurface()->getExtent();
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	const uint32_t graphicsFamilyIndex = VulkanContext::getPhysicalDevice()->getGraphicsFamilyIndex();
	const uint32_t presentFamilyIndex = VulkanContext::getPhysicalDevice()->getPresentFamilyIndex(VulkanContext::getSurface());
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

	createInfo.preTransform = VulkanContext::getSurface()->getPreTranform();
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = VulkanContext::getSurface()->getPresentMode();
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = nullptr;

	VK_CHECK(vkCreateSwapchainKHR(VulkanContext::getDevice()->getVkDevice(), &createInfo, nullptr, &m_vkSwapChain));

	vkGetSwapchainImagesKHR(VulkanContext::getDevice()->getVkDevice(), m_vkSwapChain, &imageCount, nullptr);
	m_images.resize(imageCount);
	vkGetSwapchainImagesKHR(VulkanContext::getDevice()->getVkDevice(), m_vkSwapChain, &imageCount, m_images.data());
}

void SwapChain::createImageViews()
{
	m_imageViews.resize(m_images.size());
	for (size_t i = 0; i < m_imageViews.size(); i++)
	{
		m_imageViews[i] = createImageView(m_images[i], VulkanContext::getSurface()->getImageFormat(), VK_IMAGE_ASPECT_COLOR_BIT);
	}
}

void SwapChain::cleanup()
{
	for (auto imageView : m_imageViews)
	{
		vkDestroyImageView(VulkanContext::getDevice()->getVkDevice(), imageView, nullptr);
	}

	for (auto framebuffer : m_frameBuffers)
	{
		vkDestroyFramebuffer(VulkanContext::getDevice()->getVkDevice(), framebuffer, nullptr);
	}

	vkDestroySwapchainKHR(VulkanContext::getDevice()->getVkDevice(), m_vkSwapChain, nullptr);
	
	vkDestroyImage(VulkanContext::getDevice()->getVkDevice(), m_depthImage, nullptr);
	vkFreeMemory(VulkanContext::getDevice()->getVkDevice(), m_depthImageMemory, nullptr);
	vkDestroyImageView(VulkanContext::getDevice()->getVkDevice(), m_depthImageView, nullptr);
}


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
	VkResult result = vkAcquireNextImageKHR(VulkanContext::getDevice()->getVkDevice(),
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
