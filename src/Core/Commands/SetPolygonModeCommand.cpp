#include "Core/Commands/SetPolygonModeCommand.h"

#include "Core/RenderingContextDriver/RenderingContextDriver.h"

SetPolygonModeCommand::SetPolygonModeCommand(FillMode fillMode)
	:m_fillMode(fillMode)
{
}

void SetPolygonModeCommand::record(CommandBufferID commandBuffer)
{
	RenderingContextDriver::instance()->cmdSetPolygonMode(commandBuffer, m_fillMode);
}
