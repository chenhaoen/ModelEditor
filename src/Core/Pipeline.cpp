#include "Core/Pipeline.h"
#include "Core/Camera.h"
#include "Core/Scene.h"
#include "Core/SceneManager.h"
#include "Core/Commands/CommandGroup.h"
#include "Core/Commands/BindPipelineCommand.h"
#include "Core/Commands/BindDescriptorSetsCommand.h"
#include "Core/FrameManager.h"

#include "Core/RenderingContextDriver/RenderingContextDriver.h"

Pipeline::Pipeline(const PipelineCreateInfo& pipelineCreateInfo, PipelineType type)
	:m_type(type)
{
	for (auto type : pipelineCreateInfo.m_dynamicStates)
	{
		m_dynamicStates.push_back(std::make_shared<DynamicState>(type));
	}

	if (type == PipelineType::Model)
	{
		m_id = RenderingContextDriver::instance()->createPipeline(pipelineCreateInfo);
	}
	else
	{
		m_id = RenderingContextDriver::instance()->createSkyboxPipeline(pipelineCreateInfo);
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

	for (auto dynamicState : m_dynamicStates)
	{
		commandGroup->push(dynamicState->getCommand());
	}

	return commandGroup;
}

void Pipeline::updateDescriptorSets()
{
	RenderingContextDriver::instance()->updateUniformSet(m_descriptorSets[FrameManager::instance()->currentFrameIndex()], m_boundUniforms);
}
