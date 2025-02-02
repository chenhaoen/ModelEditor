#include "VulkanContext/PipelineManager.h"
#include "VulkanContext/DescriptorSetLayout.h"
#include "VulkanContext/Pipeline.h"
#include "VulkanContext/Utils.h"

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
    Pipeline* pipeline = nullptr;
    DescriptorSetLayout* descriptorSetLayout = nullptr;
    descriptorSetLayout = new DescriptorSetLayout();
    pipeline = new Pipeline(descriptorSetLayout);

    addPipeline(g_modelPipelineName, pipeline);
}

void PipelineManager::destroy()
{
    delete g_instance;
    g_instance = nullptr;
}

Pipeline* PipelineManager::getPipeline(const std::string_view& name)
{
    auto itor = m_pipelines.find(name);
    if (itor == m_pipelines.end())
    {
        return nullptr;
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
        delete kv.second;
    }
}

void PipelineManager::addPipeline(const std::string_view& name, Pipeline* pipeline)
{
    m_pipelines.emplace(name, pipeline);
}
