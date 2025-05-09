#include "VulkanContext/PhysicalDevice.h"
#include "VulkanContext/Instance.h"
#include "VulkanContext/Surface.h"

PhysicalDevice::PhysicalDevice(
	VkPhysicalDevice vkPhysicalDevice)
	:m_vkPhysicalDevice(vkPhysicalDevice)
	, m_graphicsFamilyIndex(UINT32_MAX)
{
	vkGetPhysicalDeviceProperties(m_vkPhysicalDevice, &m_deviceProperties);
	vkGetPhysicalDeviceFeatures(m_vkPhysicalDevice, &m_deviceFeatures);

	uint32_t maxPushConstantsSize = m_deviceProperties.limits.maxPushConstantsSize;
	std::cout << "Max Push Constants Size: " << maxPushConstantsSize << " bytes" << std::endl;

	VkPhysicalDevicePushDescriptorPropertiesKHR pushDescriptorProperties = {};
	pushDescriptorProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PUSH_DESCRIPTOR_PROPERTIES_KHR;

	VkPhysicalDeviceFeatures2 features2 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2 };
	VkPhysicalDeviceExtendedDynamicStateFeaturesEXT dynamicStateFeatures = {
		VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT
	};
	features2.pNext = &dynamicStateFeatures;
	vkGetPhysicalDeviceFeatures2(m_vkPhysicalDevice, &features2);

	if (!dynamicStateFeatures.extendedDynamicState) {
		// 设备不支持动态多边形模式，回退到多管线方案
	}

	std::cout << "Max Push Descriptors: " << pushDescriptorProperties.maxPushDescriptors << std::endl;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(m_vkPhysicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(m_vkPhysicalDevice, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {

		if (m_graphicsFamilyIndex == UINT32_MAX)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				m_graphicsFamilyIndex = i;
			}
		}
		i++;
	}
}

PhysicalDevice::~PhysicalDevice()
{
}

VkPhysicalDevice PhysicalDevice::getVkPhysicalDevice() const
{
	return m_vkPhysicalDevice;
}

uint32_t PhysicalDevice::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(m_vkPhysicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

uint32_t PhysicalDevice::getGraphicsFamilyIndex() const
{
	return m_graphicsFamilyIndex;
}

uint32_t PhysicalDevice::getPresentFamilyIndex(Surface* surface) const
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(m_vkPhysicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(m_vkPhysicalDevice, &queueFamilyCount, queueFamilies.data());

	for (int i = 0; i < queueFamilyCount; ++i) {
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(m_vkPhysicalDevice, i, surface->getVkSurface(), &presentSupport);

		if (presentSupport) {
			return i;
		}
	}
	return UINT32_MAX;
}

const VkPhysicalDeviceProperties PhysicalDevice::properties() const
{
	return m_deviceProperties;
}
