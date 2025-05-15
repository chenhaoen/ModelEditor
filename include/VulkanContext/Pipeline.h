#pragma once

#include "Core/Pipeline/PipelineCreateInfo.h"

class DescriptorSetLayout;
class Pipeline
{
public:
    Pipeline(const PipelineCreateInfo& pipelineCreateInfo, DescriptorSetLayout *descriptorSetLayout);
    ~Pipeline();

    VkPipeline getVkPipeline() const;
    VkPipelineLayout getVkPipelineLayout() const;

private:
    DescriptorSetLayout *m_descriptorSetLayout;

    VkPipelineLayout m_layout;
    VkPipeline m_graphicsPipeline;
};
