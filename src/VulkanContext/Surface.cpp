#include "VulkanContext/Surface.h"
#include "VulkanContext/Instance.h"
#include "VulkanContext/VulkanContext.h"
#include "VulkanContext/Utils.h"
#include "VulkanContext/PhysicalDevice.h"

Surface::Surface(void* platformWindow)
{
#ifdef _WIN64
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.hinstance = (HINSTANCE)GetModuleHandle(NULL);
	surfaceCreateInfo.hwnd = (HWND)platformWindow;
	VK_CHECK(vkCreateWin32SurfaceKHR(VulkanContext::getVulkanInstance()->getVkInstance(), &surfaceCreateInfo, nullptr, &m_vkSurface));
#endif // _WIN64

	SwapChainSupportDetails details = querySwapChainSupport();

	m_surfaceFormat = chooseSwapSurfaceFormat(details.formats);
	m_presentMode = chooseSwapPresentMode(details.presentModes);
}

Surface::~Surface()
{
	vkDestroySurfaceKHR(VulkanContext::getVulkanInstance()->getVkInstance(), m_vkSurface, nullptr);
}

VkSurfaceKHR Surface::getVkSurface() const
{
	return m_vkSurface;
}

VkFormat Surface::getImageFormat() const
{
	return m_surfaceFormat.format;
}

VkSurfaceFormatKHR Surface::getSurfaceFormat() const
{
	return m_surfaceFormat;
}

VkPresentModeKHR Surface::getPresentMode() const
{
	return m_presentMode;
}

VkExtent2D Surface::getExtent() const
{
	SwapChainSupportDetails details = querySwapChainSupport();
	return chooseSwapExtent(details.capabilities);
}

uint32_t Surface::getImageCount() const
{
	SwapChainSupportDetails details = querySwapChainSupport();
	uint32_t imageCount = details.capabilities.minImageCount + 1;

	if (details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount)
	{
		imageCount = details.capabilities.maxImageCount;
	}
	return imageCount;
}

VkSurfaceTransformFlagBitsKHR Surface::getPreTranform() const
{
	SwapChainSupportDetails details = querySwapChainSupport();
	return details.capabilities.currentTransform;
}

SwapChainSupportDetails Surface::querySwapChainSupport() const
{
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VulkanContext::getPhysicalDevice()->getVkPhysicalDevice(), getVkSurface(), &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanContext::getPhysicalDevice()->getVkPhysicalDevice(), getVkSurface(), &formatCount, nullptr);

	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanContext::getPhysicalDevice()->getVkPhysicalDevice(), getVkSurface(), &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanContext::getPhysicalDevice()->getVkPhysicalDevice(), getVkSurface(), &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanContext::getPhysicalDevice()->getVkPhysicalDevice(), getVkSurface(),
			&presentModeCount, details.presentModes.data());
	}

	return details;
}

VkSurfaceFormatKHR Surface::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const
{
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR Surface::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const
{
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Surface::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const
{
	return capabilities.currentExtent;
}
