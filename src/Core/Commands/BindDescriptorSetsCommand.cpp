#include "Core/Commands/BindDescriptorSetsCommand.h"
#include "Core/FrameManager.h"
#include "Core/Pipeline.h"
#include "Core/PipelineManager.h"

#include "Core/RenderingContextDriver/RenderingContextDriver.h"

BindDescriptorSetsCommand::BindDescriptorSetsCommand(PipelineType type)
	:m_type(type)
{
}

void BindDescriptorSetsCommand::record()
{
	RenderingContextDriver::instance()->cmdBindDescriptorSets(
		FrameManager::instance()->currentCommandBuffer(),
		PipelineManager::instance()->getPipeline(m_type)->m_id,
		PipelineManager::instance()->getPipeline(m_type)->m_descriptorSets[FrameManager::instance()->currentFrameIndex()]
	);
}
