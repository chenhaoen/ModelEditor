#include "Core/Commands/DrawCommand.h"
#include "Core/FrameManager.h"

#include "Core/RenderingContextDriver/RenderingContextDriver.h"

DrawCommand::DrawCommand(
	DrawMode mode, 
	uint32_t indexCount,
	uint32_t instanceCount,
	uint32_t firstIndex, 
	int32_t vertexOffset, 
	uint32_t firstInstance)
	:m_indexCount(indexCount)
	,m_instanceCount(instanceCount)
	,m_firstIndex(firstIndex)
	,m_vertexOffset{vertexOffset}
	,m_firstInstance{firstInstance}
	,m_mode(mode)
{
}

DrawCommand::~DrawCommand()
{
}

void DrawCommand::record()
{
	switch (m_mode)
	{
	case DrawMode::Indexed:
		RenderingContextDriver::instance()->cmdDrawIndexed(FrameManager::instance()->currentCommandBuffer(),
			m_indexCount, m_instanceCount, m_firstIndex, m_vertexOffset, m_firstInstance);
		break;
	case DrawMode::NonIndexed:
		RenderingContextDriver::instance()->cmdDraw(FrameManager::instance()->currentCommandBuffer(),
			m_indexCount, m_instanceCount, m_firstIndex, m_firstInstance);
		break;
	default:
		break;
	}

}
