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
		physicalDevice->getPresentFamilyIndex(VulkanContext::getSurface()) };

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
	deviceFeatures.fillModeNonSolid = true;
	createInfo.pEnabledFeatures = &deviceFeatures;

	VkPhysicalDeviceExtendedDynamicState3FeaturesEXT dynamicState3Features = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_3_FEATURES_EXT,
		.extendedDynamicState3PolygonMode = VK_TRUE,
	};

	VkPhysicalDeviceFeatures2 features2 = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
		.pNext = &dynamicState3Features,
		.features = deviceFeatures,
	};

	createInfo.pNext = &features2;

	createInfo.enabledExtensionCount = VulkanContext::getDeviceExtensions().size();
	createInfo.ppEnabledExtensionNames = VulkanContext::getDeviceExtensions().data();

	if (VulkanContext::instance()->getInitInfo().m_debug) {
		createInfo.enabledLayerCount = VulkanContext::getInstanceLayers().size();
		createInfo.ppEnabledLayerNames = VulkanContext::getInstanceLayers().data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	VK_CHECK(vkCreateDevice(physicalDevice->getVkPhysicalDevice(), &createInfo, nullptr, &m_vkDevice));

	vkGetDeviceQueue(m_vkDevice, physicalDevice->getGraphicsFamilyIndex(), 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_vkDevice, physicalDevice->getPresentFamilyIndex(VulkanContext::getSurface()), 0, &m_presentQueue);
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

void LogicalDevice::wait() const
{
	vkDeviceWaitIdle(m_vkDevice);
}
