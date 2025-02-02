#pragma once

#include "VulkanContext/Command.h"

class Pipeline;

class PipelineCommand :
	public Command
{
public:
	PipelineCommand(Pipeline* pipeline);

	void record(VkCommandBuffer commandBuffer) override;

private:
	Pipeline* m_pipeline;
};

