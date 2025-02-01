#pragma once

#include <vulkan/vulkan.h>

class Surface;
class PhysicalDevice
{
public:
	PhysicalDevice(VkPhysicalDevice vkPhysicalDevice);
	~PhysicalDevice();

	VkPhysicalDevice getVkPhysicalDevice() const;

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	uint32_t getGraphicsFamilyIndex() const;
	uint32_t getPresentFamilyIndex(Surface* surface) const;
private:
	VkPhysicalDevice m_vkPhysicalDevice;

	VkPhysicalDeviceProperties m_deviceProperties;
	VkPhysicalDeviceFeatures m_deviceFeatures;

	uint32_t m_graphicsFamilyIndex;
};

