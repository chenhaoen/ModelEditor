#pragma once

#include "VulkanContext/Command.h"

class DrawCommand : public Command
{
public:
	DrawCommand(
		uint32_t vertexCount,
		uint32_t instanceCount,
		uint32_t firstVertex,
		uint32_t firstInstance);

	void record(VkCommandBuffer commandBuffer) override;

private:
	uint32_t m_vertexCount;
	uint32_t m_instanceCount;
	uint32_t m_firstVertex;
	uint32_t m_firstInstance;
};

