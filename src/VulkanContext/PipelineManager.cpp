#include "VulkanContext/PipelineManager.h"

PipelineManager* PipelineManager::g_instance = nullptr;

PipelineManager* PipelineManager::instance()
{
    if (!g_instance)
    {
        g_instance = new PipelineManager();
    }

    return g_instance;
}

void PipelineManager::destroy()
{
    delete g_instance;
    g_instance = nullptr;
}

PipelineManager::PipelineManager()
{

}

void PipelineManager::AddPipeline(const std::string_view& name, Pipeline* pipeline)
{
    m_pipelines.emplace(name, pipeline);
}
