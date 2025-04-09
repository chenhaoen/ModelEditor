#include "Core/Commands/SetScissorCommand.h"
#include "Core/FrameManager.h"

#include "Core/RenderingContextDriver/RenderingContextDriver.h"

SetScissorCommand::SetScissorCommand(const Rect2D& scissor)
	:m_scissor(scissor)
{

}

void SetScissorCommand::record()
{
	RenderingContextDriver::instance()->cmdSetScissor(FrameManager::instance()->currentCommandBuffer(), m_scissor);
}
