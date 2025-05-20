#pragma once

class Shader;

class DescriptorSetLayout
{
public:
    DescriptorSetLayout(const std::list<std::shared_ptr<Shader>>&);
    ~DescriptorSetLayout();

    VkDescriptorSetLayout getVkDescriptorSetLayout() const;

private:
    VkDescriptorSetLayout m_descriptorSetLayout;
};