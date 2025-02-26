#include <stdexcept>
#include <array>

#include "VulkanContext/DescriptorSetLayout.h"
#include "VulkanContext/LogicalDevice.h"
#include "VulkanContext/VulkanContext.h"
#include "VulkanContext/Utils.h"

DescriptorSetLayout::DescriptorSetLayout()
{
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

    //VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    //samplerLayoutBinding.binding = 1;
    //samplerLayoutBinding.descriptorCount = 1;
    //samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    //samplerLayoutBinding.pImmutableSamplers = nullptr;
    //samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 1> bindings = {uboLayoutBinding};

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