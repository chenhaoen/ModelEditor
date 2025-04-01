#include "Core/Commands/SetPolygonModeCommand.h"

#include "RenderingContextDriver/RenderingContextDriver.h"
#include "Core/FrameManager.h"

SetPolygonModeCommand::SetPolygonModeCommand(FillMode fillMode)
	:m_fillMode(fillMode)
{
}

void SetPolygonModeCommand::record()
{
	RenderingContextDriver::instance()->cmdSetPolygonMode(FrameManager::instance()->currentCommandBuffer(), m_fillMode);
}
