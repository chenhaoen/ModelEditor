#pragma once

class PhysicalDevice;
class Instance
{
public:
	Instance();

	~Instance();

	VkInstance getVkInstance() const;

	PhysicalDevice* getBestPhysicalDevice();

private:
	VkInstance m_vkInstance;

	std::vector<VkExtensionProperties> m_extensionProperties;

	std::vector<VkLayerProperties> m_availableLayers;
};

