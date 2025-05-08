#include <iostream>

#include "Core/Commands/BindPipelineCommand.h"
#include "Core/FrameManager.h"
#include "Core/PipelineManager.h"
#include "Core/Pipeline.h"

#include "Core/RenderingContextDriver/RenderingContextDriver.h"

BindPipelineCommand::BindPipelineCommand(PipelineType type)
	:Command(),
	m_type(type)
{
}

void BindPipelineCommand::record()
{
	RenderingContextDriver::instance()->cmdBindPipeline(FrameManager::instance()->currentCommandBuffer(), PipelineManager::instance()->getPipeline(m_type)->m_id);
}
