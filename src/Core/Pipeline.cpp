#include "Core/Pipeline.h"
#include "Core/Scene.h"
#include "Core/SceneManager.h"
#include "Core/Camera.h"
#include "Core/Commands/SetScissorCommand.h"
#include "Core/Commands/SetViewportCommand.h"
#include "Core/Commands/CommandGroup.h"
#include "Core/Commands/BindPipelineCommand.h"
#include "Core/Commands/BindDescriptorSetsCommand.h"
#include "Core/Commands/SetPolygonModeCommand.h"
#include "Core/FrameManager.h"

#include "Core/RenderingContextDriver/RenderingContextDriver.h"

Pipeline::Pipeline(PipelineType type)
	:m_type(type)
{
	if (type == PipelineType::Model)
	{
		m_id = RenderingContextDriver::instance()->createPipeline();
	}
	else
	{
		m_id = RenderingContextDriver::instance()->createSkyboxPipeline();
	}


	for (auto& descriptorSet : m_descriptorSets)
	{
		descriptorSet = RenderingContextDriver::instance()->createUniformSet(m_id);
	}
}

Pipeline::~Pipeline()
{
	for (auto& descriptorSet : m_descriptorSets)
	{
		RenderingContextDriver::instance()->freeUniformSet(descriptorSet);
	}

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
	commandGroup->push(std::make_shared<SetPolygonModeCommand>(SceneManager::instance()->getCurrentScene()->getFillMode()));

	return commandGroup;
}

void Pipeline::updateDescriptorSets()
{
	RenderingContextDriver::instance()->updateUniformSet(m_descriptorSets[FrameManager::instance()->currentFrameIndex()], m_boundUniforms);
}
