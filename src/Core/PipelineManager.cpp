#include "Core/PipelineManager.h"
#include "Core/Pipeline.h"

#include "Core/RenderingContextDriver/RenderingContextDriver.h"

PipelineManager* PipelineManager::g_instance = nullptr;

PipelineManager* PipelineManager::instance()
{
	if (!g_instance)
	{
		g_instance = new PipelineManager();
	}

	return g_instance;
}

void PipelineManager::init()
{
	{
		PipelineCreateInfo pipelineCreateInfo;
		pipelineCreateInfo.m_dynamicStates.push_back(DynamicStateType::Scissor);
		pipelineCreateInfo.m_dynamicStates.push_back(DynamicStateType::Viewport);
		pipelineCreateInfo.m_dynamicStates.push_back(DynamicStateType::PolygonMode);
		pipelineCreateInfo.m_renderPrimitive = RenderPrimitive::RENDER_PRIMITIVE_TRIANGLES;
		auto pipeline = std::make_shared<Pipeline>(pipelineCreateInfo,PipelineType::Model);
		addPipeline(PipelineType::Model, pipeline);
	}

	// skybox
	{
		PipelineCreateInfo pipelineCreateInfo;
		pipelineCreateInfo.m_dynamicStates.push_back(DynamicStateType::Scissor);
		pipelineCreateInfo.m_dynamicStates.push_back(DynamicStateType::Viewport);
		pipelineCreateInfo.m_renderPrimitive = RenderPrimitive::RENDER_PRIMITIVE_TRIANGLES;
		auto pipeline = std::make_shared<Pipeline>(pipelineCreateInfo,PipelineType::Skybox);
		addPipeline(PipelineType::Skybox, pipeline);
	}
}

std::shared_ptr<Pipeline> PipelineManager::getPipeline(const PipelineType type)
{
	return m_pipelines[type];
}

void PipelineManager::destroy()
{
	delete g_instance;
	g_instance = nullptr;
}

uint32_t PipelineManager::pipelineCount() const
{
	return m_pipelines.size();
}

std::shared_ptr<CommandGroup> PipelineManager::getCommands(const PipelineType type)
{
	return m_pipelines[type]->getCommands();
}

void PipelineManager::updateDescriptorSets(const PipelineType type)
{
	m_pipelines[type]->updateDescriptorSets();
}

PipelineManager::PipelineManager()
{

}

PipelineManager::~PipelineManager()
{
}

void PipelineManager::addPipeline(const PipelineType type, std::shared_ptr<Pipeline> pipeline)
{
	m_pipelines.emplace(type, pipeline);
}
