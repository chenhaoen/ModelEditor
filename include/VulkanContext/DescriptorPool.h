#pragma once

#include <vulkan/vulkan.h>

class DescriptorSetLayout;
class DescriptorPool
{
public:
    DescriptorPool();
    ~DescriptorPool();

    VkDescriptorPool getVkDescriptorPool() const;

    VkDescriptorSet AllocateDescriptorSet(DescriptorSetLayout* layout) const;
private:
    VkDescriptorPool m_vkDescriptorPool;
};

