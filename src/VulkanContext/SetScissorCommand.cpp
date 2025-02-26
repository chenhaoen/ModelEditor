#include "VulkanContext/SetScissorCommand.h"

SetScissorCommand::SetScissorCommand(const VkRect2D& scissor)
	:m_scissor(scissor)
{
}

void SetScissorCommand::record(VkCommandBuffer commandBuffer)
{
	vkCmdSetScissor(commandBuffer, 0, 1, &m_scissor);
}
