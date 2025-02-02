#include "VulkanContext/PipelineCommand.h"

#include "VulkanContext/Pipeline.h"

PipelineCommand::PipelineCommand(Pipeline* pipeline)
	:m_pipeline(pipeline)
{
}

void PipelineCommand::record(VkCommandBuffer commandBuffer)
{
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->getVkPipeline());
}
