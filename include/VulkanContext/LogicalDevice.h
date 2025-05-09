#pragma once

class PhysicalDevice;
class LogicalDevice
{
public:
	LogicalDevice(PhysicalDevice* physicalDevice);
	~LogicalDevice();

	VkDevice getVkDevice() const;

	VkQueue getGraphicsQueue() const;
	VkQueue getPresentQueue() const;

	void wait() const;
private:
	VkDevice m_vkDevice;

	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;
};

