#pragma once

class DescriptorSetLayout;
class Shader;

class DescriptorPool
{
public:
    DescriptorPool(const std::list<std::shared_ptr<Shader>>&);
    ~DescriptorPool();

    VkDescriptorPool getVkDescriptorPool() const;

    VkDescriptorSet AllocateDescriptorSet(DescriptorSetLayout* layout) const;

    void freeDescriptorSets(const std::vector<VkDescriptorSet>& descriptorSets);
private:
    VkDescriptorPool m_vkDescriptorPool;
};

