#include "Core/Commands/SetViewportCommand.h"
#include "Core/FrameManager.h"

#include "RenderingContextDriver/RenderingContextDriver.h"

SetViewportCommand::SetViewportCommand(const Viewport& viewport)
	:m_viewport(viewport)
{
}

void SetViewportCommand::record()
{
	RenderingContextDriver::instance()->cmdSetViewport(FrameManager::instance()->currentCommandBuffer(), m_viewport);
}
