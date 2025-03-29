#include "Core/PipelineManager.h"
#include "Core/Pipeline.h"

#include "RenderingContextDriver/RenderingContextDriver.h"

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
		auto pipeline = std::make_shared<Pipeline>(PipelineType::Model);
		pipeline->m_id = RenderingContextDriver::instance()->createPipeline();
		addPipeline(PipelineType::Model, pipeline);
	}

}

void PipelineManager::setCurrentPipeline(const PipelineType type)
{
	m_currentPipeline = type;
	m_pipelines[m_currentPipeline]->m_boundUniforms.clear();
	m_pipelines[m_currentPipeline]->bind();
}

std::shared_ptr<Pipeline> PipelineManager::currentPipeline()
{
	return m_pipelines[m_currentPipeline];
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

std::shared_ptr<CommandGroup> PipelineManager::getCommands()
{
	return m_pipelines[m_currentPipeline]->getCommands();
}

void PipelineManager::updateDescriptorSets()
{
	m_pipelines[m_currentPipeline]->updateDescriptorSets();
}

PipelineManager::PipelineManager()
	:m_currentPipeline(PipelineType::Model)
{

}

PipelineManager::~PipelineManager()
{
}

void PipelineManager::addPipeline(const PipelineType type, std::shared_ptr<Pipeline> pipeline)
{
	m_pipelines.emplace(type, pipeline);
}
