#pragma once

class DescriptorSetLayout;
class Pipeline
{
public:
    Pipeline(const std::string& vertShaderFile, const std::string& fragShaderFile, DescriptorSetLayout *descriptorSetLayout);
    ~Pipeline();

    static std::vector<char> readFile(const std::string &filename);

    VkShaderModule createShaderModule(const std::vector<char> &code);

    VkPipeline getVkPipeline() const;
    VkPipelineLayout getVkPipelineLayout() const;

private:
    VkPipelineDynamicStateCreateInfo getDynamicStateCreateInfo() const;

    VkPipelineInputAssemblyStateCreateInfo getInputAssemblyStateCreateInfo() const;

    VkPipelineVertexInputStateCreateInfo getVertexInputStateCreateInfo() const;

    VkPipelineViewportStateCreateInfo getViewportStateCreateInfo() const;
private:
    DescriptorSetLayout *m_descriptorSetLayout;

    VkPipelineLayout m_layout;
    VkPipeline m_graphicsPipeline;
};
