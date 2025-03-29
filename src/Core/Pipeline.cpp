#include "Core/Pipeline.h"
#include "Core/Scene.h"
#include "Core/SceneManager.h"
#include "Core/Camera.h"
#include "Core/Commands/SetScissorCommand.h"
#include "Core/Commands/SetViewportCommand.h"
#include "Core/Commands/CommandGroup.h"
#include "Core/Commands/BindPipelineCommand.h"
#include "Core/Commands/BindDescriptorSetsCommand.h"
#include "Core/FrameManager.h"

#include "RenderingContextDriver/RenderingContextDriver.h"

Pipeline::Pipeline(PipelineType type)
	:m_type(type)
{
}

Pipeline::~Pipeline()
{
	RenderingContextDriver::instance()->freePipeline(m_id);
}

void Pipeline::bind()
{
	m_boundUniforms.push_back(SceneManager::instance()->getCurrentScene()->getCurrentCamera()->getUniform());
}

std::shared_ptr<CommandGroup> Pipeline::getCommands()
{
	auto commandGroup = std::make_shared<CommandGroup>();

	const Extent2D& surfaceExtent = RenderingContextDriver::instance()->getSurfaceExtent(SurfaceID());

	commandGroup->push(std::make_shared<BindPipelineCommand>());

	Rect2D scissor;
	scissor.offset = { 0,0 };
	scissor.extent = surfaceExtent;
	commandGroup->push(std::make_shared<SetScissorCommand>(scissor));

	Viewport viewport;
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)surfaceExtent.width;
	viewport.height = (float)surfaceExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	commandGroup->push(std::make_shared<SetViewportCommand>(viewport));

	commandGroup->push(std::make_shared < BindDescriptorSetsCommand>());

	return commandGroup;
}

void Pipeline::updateDescriptorSets()
{
	RenderingContextDriver::instance()->updateUniformSet(FrameManager::instance()->currentUniformSet(), m_boundUniforms);
}
