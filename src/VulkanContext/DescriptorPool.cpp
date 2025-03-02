#include "VulkanContext/DescriptorPool.h"
#include "VulkanContext/LogicalDevice.h"
#include "VulkanContext/VulkanContext.h"
#include "VulkanContext/Utils.h"
#include "VulkanContext/DescriptorSetLayout.h"

#include "RenderingContextDriver/FrameManager.h"

DescriptorPool::DescriptorPool()
{
	VkDescriptorPoolSize poolSize{};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = static_cast<uint32_t>(FrameManager::maxFrameCount());

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;

	poolInfo.maxSets = static_cast<uint32_t>(FrameManager::maxFrameCount());

	VK_CHECK(vkCreateDescriptorPool(VulkanContext::getDevice()->getVkDevice(), &poolInfo, nullptr, &m_vkDescriptorPool))
}

DescriptorPool::~DescriptorPool()
{
	vkDestroyDescriptorPool(VulkanContext::getDevice()->getVkDevice(), m_vkDescriptorPool,nullptr);
}

VkDescriptorPool DescriptorPool::getVkDescriptorPool() const
{
	return m_vkDescriptorPool;
}

VkDescriptorSet DescriptorPool::AllocateDescriptorSet(DescriptorSetLayout* layout) const
{
	std::vector< VkDescriptorSetLayout > layouts; 
	layouts.emplace_back(layout->getVkDescriptorSetLayout());

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = m_vkDescriptorPool;
	allocInfo.descriptorSetCount = layouts.size();
	allocInfo.pSetLayouts = layouts.data();

	VkDescriptorSet result;
	VK_CHECK(vkAllocateDescriptorSets(VulkanContext::getDevice()->getVkDevice(), &allocInfo, &result));
	return result;
}
