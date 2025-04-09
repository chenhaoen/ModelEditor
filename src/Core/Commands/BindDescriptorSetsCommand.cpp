#include "Core/Commands/BindDescriptorSetsCommand.h"
#include "Core/FrameManager.h"
#include "Core/Pipeline.h"
#include "Core/PipelineManager.h"

#include "Core/RenderingContextDriver/RenderingContextDriver.h"

BindDescriptorSetsCommand::BindDescriptorSetsCommand()
{
}

void BindDescriptorSetsCommand::record()
{
	RenderingContextDriver::instance()->cmdBindDescriptorSets(
		FrameManager::instance()->currentCommandBuffer(),
		PipelineManager::instance()->currentPipeline()->m_id,
		FrameManager::instance()->currentUniformSet()
	);
}
