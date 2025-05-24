#include "Core/Commands/SetViewportCommand.h"

#include "Core/RenderingContextDriver/RenderingContextDriver.h"

SetViewportCommand::SetViewportCommand(const Viewport& viewport)
	:m_viewport(viewport)
{
}

void SetViewportCommand::record(CommandBufferID commandBuffer)
{
	RenderingContextDriver::instance()->cmdSetViewport(commandBuffer, m_viewport);
}
