#include "VulkanContext/SetViewportCommand.h"

SetViewportCommand::SetViewportCommand(const VkViewport& viewport)
	:m_viewport(viewport)
{
}

void SetViewportCommand::record(VkCommandBuffer commandBuffer)
{
	vkCmdSetViewport(commandBuffer, 0, 1, &m_viewport);
}

