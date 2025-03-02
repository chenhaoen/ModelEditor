#include "RenderingContextDriver/PipelineManager.h"
#include "RenderingContextDriver/RenderingContextDriver.h"

PipelineManager* PipelineManager::g_instance = nullptr;

PipelineManager* PipelineManager::instance()
{
	if (!g_instance)
	{
		g_instance = new PipelineManager();
		g_instance->init();
	}

	return g_instance;
}

void PipelineManager::init()
{
	addPipeline(PipelineType::Model, RenderingContextDriver::instance()->createPipeline());
}

void PipelineManager::destroy()
{
	delete g_instance;
	g_instance = nullptr;
}

PipelineID PipelineManager::getPipeline(const PipelineType type)
{
	auto itor = m_pipelines.find(type);
	if (itor == m_pipelines.end())
	{
		return PipelineID();
	}

	return itor->second;
}

PipelineManager::PipelineManager()
{

}

PipelineManager::~PipelineManager()
{
	for (auto kv : m_pipelines)
	{
		RenderingContextDriver::instance()->freePipeline(kv.second);
	}
}

void PipelineManager::addPipeline(const PipelineType type, PipelineID pipeline)
{
	m_pipelines.emplace(type, pipeline);
}
