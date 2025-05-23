#include "VulkanContext/DescriptorSetLayout.h"
#include "VulkanContext/LogicalDevice.h"
#include "VulkanContext/VulkanContext.h"
#include "VulkanContext/Utils.h"

#include "Core/Shader/Descriptor.h"

DescriptorSetLayout::DescriptorSetLayout(const std::list<std::shared_ptr<Shader>>& shaders)
{
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    for (auto shader : shaders)
    {
        for (auto descriptor : shader->getDescriptors())
        {
            VkDescriptorSetLayoutBinding binding{};
            binding.binding = descriptor->getBinding();
            binding.descriptorType = UniformTypeToVK(descriptor->getUniformType());
            binding.descriptorCount = 1;
            binding.stageFlags = ShaderTypeToVk(shader->getType());
            binding.pImmutableSamplers = nullptr; // Optional
            bindings.push_back(binding);
        }
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = bindings.size();
    layoutInfo.pBindings = bindings.data();

    VK_CHECK(vkCreateDescriptorSetLayout(VulkanContext::getDevice()->getVkDevice(), &layoutInfo, nullptr, &m_descriptorSetLayout));
}

DescriptorSetLayout::~DescriptorSetLayout()
{
    vkDestroyDescriptorSetLayout(VulkanContext::getDevice()->getVkDevice(), m_descriptorSetLayout, nullptr);
}

VkDescriptorSetLayout DescriptorSetLayout::getVkDescriptorSetLayout() const
{
    return m_descriptorSetLayout;
}