#include "Core/Commands/BindBufferCommand.h"
#include "Core/FrameManager.h"

#include "Core/RenderingContextDriver/RenderingContextDriver.h"

BindBufferCommand::BindBufferCommand(BufferID buffer, BufferType type)
	: m_buffer(buffer)
	, m_bufferType(type)
{
}

void BindBufferCommand::record()
{
	switch (m_bufferType)
	{
	case BufferType::Vertex: 
		RenderingContextDriver::instance()->cmdBindVertexBuffer(FrameManager::instance()->currentCommandBuffer(), m_buffer);
		break;
	case BufferType::Index:
		RenderingContextDriver::instance()->cmdBindIndexBuffer(FrameManager::instance()->currentCommandBuffer(), m_buffer);
		break;
	default:
		break;
	}
}
