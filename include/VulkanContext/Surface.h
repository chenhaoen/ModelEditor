#pragma once

#include <vulkan/vulkan.h>

class Surface
{
public:
	Surface(void* platformWindow);
	~Surface();

	VkSurfaceKHR getVkSurface() const;
private:
	VkSurfaceKHR m_vkSurface;
};

