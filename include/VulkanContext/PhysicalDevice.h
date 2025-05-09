#pragma once

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

	const VkPhysicalDeviceProperties properties() const;
private:
	VkPhysicalDevice m_vkPhysicalDevice;

	VkPhysicalDeviceProperties m_deviceProperties;
	VkPhysicalDeviceFeatures m_deviceFeatures;

	uint32_t m_graphicsFamilyIndex;
};

