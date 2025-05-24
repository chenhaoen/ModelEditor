#include "Core/Commands/SetScissorCommand.h"

#include "Core/RenderingContextDriver/RenderingContextDriver.h"

SetScissorCommand::SetScissorCommand(const Rect2D& scissor)
	:m_scissor(scissor)
{

}

void SetScissorCommand::record(CommandBufferID commandBuffer)
{
	RenderingContextDriver::instance()->cmdSetScissor(commandBuffer, m_scissor);
}
