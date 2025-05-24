#include "Core/Commands/BindPipelineCommand.h"
#include "Core/FrameManager.h"
#include "Core/PipelineManager.h"
#include "Core/Pipeline.h"

#include "Core/RenderingContextDriver/RenderingContextDriver.h"

BindPipelineCommand::BindPipelineCommand(PipelineName name)
	:Command(),
	m_name(name)
{
}

void BindPipelineCommand::record(CommandBufferID commandBuffer)
{
	RenderingContextDriver::instance()->cmdBindPipeline(commandBuffer, PipelineManager::instance()->getPipeline(m_name)->m_id);
}
