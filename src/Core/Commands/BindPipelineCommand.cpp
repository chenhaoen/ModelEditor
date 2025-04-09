#include "Core/Commands/BindPipelineCommand.h"
#include "Core/FrameManager.h"
#include "Core/PipelineManager.h"
#include "Core/Pipeline.h"

#include "Core/RenderingContextDriver/RenderingContextDriver.h"

BindPipelineCommand::BindPipelineCommand()
{
}

void BindPipelineCommand::record()
{
	RenderingContextDriver::instance()->cmdBindPipeline(FrameManager::instance()->currentCommandBuffer(), PipelineManager::instance()->currentPipeline()->m_id);
}
