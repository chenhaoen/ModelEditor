#pragma once

#include <vulkan/vulkan.h>

class RenderPass
{
public:
	RenderPass();
	~RenderPass();

	VkRenderPass getVkRenderPass() const;

private:
	VkRenderPass m_renderPass;
};

