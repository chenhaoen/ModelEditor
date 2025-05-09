#pragma once

class DescriptorSetLayout
{
public:
    DescriptorSetLayout();
    ~DescriptorSetLayout();

    VkDescriptorSetLayout getVkDescriptorSetLayout() const;

private:
    VkDescriptorSetLayout m_descriptorSetLayout;
};