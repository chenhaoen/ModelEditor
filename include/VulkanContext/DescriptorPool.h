#pragma once

class DescriptorSetLayout;
class DescriptorPool
{
public:
    DescriptorPool();
    ~DescriptorPool();

    VkDescriptorPool getVkDescriptorPool() const;

    VkDescriptorSet AllocateDescriptorSet(DescriptorSetLayout* layout) const;

    void freeDescriptorSets(const std::vector<VkDescriptorSet>& descriptorSets);
private:
    VkDescriptorPool m_vkDescriptorPool;
};

