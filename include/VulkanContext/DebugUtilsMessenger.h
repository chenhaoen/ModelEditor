#pragma once

class DebugUtilsMessenger
{
public:
	DebugUtilsMessenger();
	~DebugUtilsMessenger();

	static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

private:
	VkDebugUtilsMessengerEXT m_vkDebugUtilsMessenger;
};

