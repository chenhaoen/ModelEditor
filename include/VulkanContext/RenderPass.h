#pragma once

class RenderPass
{
public:
	RenderPass();
	~RenderPass();

	VkRenderPass getVkRenderPass() const;

private:
	VkRenderPass m_renderPass;
};

