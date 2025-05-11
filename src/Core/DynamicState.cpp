#include "Core/DynamicState.h"
#include "Core/Scene.h"
#include "Core/SceneManager.h"

#include "Core/RenderingContextDriver/RenderingContextDriver.h"

#include "Core/Commands/SetScissorCommand.h"
#include "Core/Commands/SetPolygonModeCommand.h"
#include "Core/Commands/SetViewportCommand.h"

DynamicState::DynamicState(DynamicStateType type)
	:m_type(type)
{
}

std::shared_ptr<Command> DynamicState::getCommand() const
{
	std::shared_ptr<Command> command;

	const Extent2D& surfaceExtent = RenderingContextDriver::instance()->getSurfaceExtent(SurfaceID());

	switch (m_type)
	{
	case DynamicStateType::Scissor:
	{
		Rect2D scissor;
		scissor.offset = { 0,0 };
		scissor.extent = surfaceExtent;
		command = std::make_shared<SetScissorCommand>(scissor);
	}
		break;
	case DynamicStateType::Viewport:
	{
		Viewport viewport;
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)surfaceExtent.width;
		viewport.height = (float)surfaceExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		command = std::make_shared<SetViewportCommand>(viewport);
	}
		break;
	case DynamicStateType::PolygonMode:
	{
		command = std::make_shared<SetPolygonModeCommand>(SceneManager::instance()->getCurrentScene()->getFillMode());
	}
		break;
	default:
		break;
	}

	return command;
}
