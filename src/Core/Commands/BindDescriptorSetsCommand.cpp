#include "Core/Commands/BindDescriptorSetsCommand.h"
#include "Core/FrameManager.h"
#include "Core/Pipeline.h"
#include "Core/PipelineManager.h"

#include "Core/RenderingContextDriver/RenderingContextDriver.h"

BindDescriptorSetsCommand::BindDescriptorSetsCommand(PipelineName name)
	:m_name(name)
{
}

void BindDescriptorSetsCommand::record(CommandBufferID commandBuffer)
{
	RenderingContextDriver::instance()->cmdBindDescriptorSets(
		commandBuffer,
		PipelineManager::instance()->getPipeline(m_name)->m_id,
		PipelineManager::instance()->getPipeline(m_name)->m_descriptorSets[FrameManager::instance()->currentFrameIndex()]
	);
}
