#include "VulkanContext/DrawCommand.h"

DrawCommand::DrawCommand(
	uint32_t vertexCount,
	uint32_t instanceCount,
	uint32_t firstVertex,
	uint32_t firstInstance)
	:m_vertexCount(vertexCount)
	, m_instanceCount(instanceCount)
	, m_firstVertex(firstVertex)
	, m_firstInstance(firstInstance)
{
}

void DrawCommand::record(VkCommandBuffer commandBuffer)
{
	vkCmdDraw(commandBuffer, m_vertexCount, m_instanceCount, m_firstVertex, m_firstInstance);
}
