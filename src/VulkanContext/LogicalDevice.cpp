#include <set>
#include <stdexcept>

#include "VulkanContext/LogicalDevice.h"
#include "VulkanContext/PhysicalDevice.h"
#include "VulkanContext/VulkanContext.h"
#include "VulkanContext/Utils.h"

LogicalDevice::LogicalDevice(PhysicalDevice* physicalDevice)
{
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { physicalDevice->getGraphicsFamilyIndex(),
		physicalDevice->getPresentFamilyIndex(VulkanContext::instance()->getSurface()) };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = queueCreateInfos.size();

	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = true;
	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = VulkanContext::instance()->getDeviceExtensions().size();
	createInfo.ppEnabledExtensionNames = VulkanContext::instance()->getDeviceExtensions().data();

	if (VulkanContext::instance()->getInitInfo().m_debug) {
		createInfo.enabledLayerCount = VulkanContext::instance()->getInstanceLayers().size();
		createInfo.ppEnabledLayerNames = VulkanContext::instance()->getInstanceLayers().data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	VK_CHECK(vkCreateDevice(physicalDevice->getVkPhysicalDevice(), &createInfo, nullptr, &m_vkDevice));

	vkGetDeviceQueue(m_vkDevice, physicalDevice->getGraphicsFamilyIndex(), 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_vkDevice, physicalDevice->getPresentFamilyIndex(VulkanContext::instance()->getSurface()), 0, &m_presentQueue);
}

LogicalDevice::~LogicalDevice()
{
	vkDestroyDevice(m_vkDevice, nullptr);
}

VkDevice LogicalDevice::getVkDevice() const
{
	return m_vkDevice;
}

VkQueue LogicalDevice::getGraphicsQueue() const
{
	return m_graphicsQueue;
}

VkQueue LogicalDevice::getPresentQueue() const
{
	return m_presentQueue;
}

void LogicalDevice::Wait() const
{
	vkDeviceWaitIdle(m_vkDevice);
}
