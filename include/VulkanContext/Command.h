#pragma once

#include <vulkan/vulkan.h>

class Command
{
public:
	virtual void record(VkCommandBuffer commandBuffer) = 0;
};

