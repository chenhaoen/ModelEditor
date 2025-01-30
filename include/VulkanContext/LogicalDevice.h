#pragma once

#include <vulkan/vulkan.h>

class PhysicalDevice;
class LogicalDevice
{
public:
	LogicalDevice(PhysicalDevice* physicalDevice);
	~LogicalDevice();

	VkDevice getVkDevice() const;

	VkQueue getGraphicsQueue() const;
	VkQueue getPresentQueue() const;

	void Wait() const;
private:
	VkDevice m_vkDevice;

	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;
};

