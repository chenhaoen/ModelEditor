#pragma once

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class Surface
{
public:
	Surface(void* platformWindow);
	~Surface();

	VkSurfaceKHR getVkSurface() const;

	VkFormat getImageFormat() const;
	VkSurfaceFormatKHR getSurfaceFormat() const;
	VkPresentModeKHR getPresentMode() const;
	VkExtent2D getExtent() const;
	uint32_t getImageCount() const;
	VkSurfaceTransformFlagBitsKHR getPreTranform() const;

	SwapChainSupportDetails querySwapChainSupport() const;

private:
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;

private:
	VkSurfaceKHR m_vkSurface;

	VkSurfaceFormatKHR m_surfaceFormat;

	VkPresentModeKHR m_presentMode;
};

