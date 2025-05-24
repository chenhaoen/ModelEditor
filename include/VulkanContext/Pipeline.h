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

    PipelineType getType() const;

private:
    void createGraphicsPipeline(const PipelineCreateInfo& pipelineCreateInfo, DescriptorSetLayout* descriptorSetLayout);

    void createComputePipeline(const PipelineCreateInfo& pipelineCreateInfo, DescriptorSetLayout* descriptorSetLayout);
private:
    DescriptorSetLayout *m_descriptorSetLayout;

    VkPipelineLayout m_layout;
    VkPipeline m_pipeline;

    PipelineType m_type;
};
