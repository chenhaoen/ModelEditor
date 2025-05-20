#include "VulkanContext/DescriptorPool.h"
#include "VulkanContext/LogicalDevice.h"
#include "VulkanContext/VulkanContext.h"

#include "VulkanContext/Utils.h"
#include "VulkanContext/DescriptorSetLayout.h"

#include "Core/FrameManager.h"
#include "Core/Shader/Shader.h"
#include "Core/Shader/Descriptor.h"

DescriptorPool::DescriptorPool(const std::list<std::shared_ptr<Shader>>& shaders)
{


	std::map<VkDescriptorType, uint32_t> descriptorCount;
	for (auto shader : shaders)
	{
		for (auto descriptor : shader->getDescriptors())
		{
			VkDescriptorType type = UniformTypeToVK(descriptor->getUniformType());

			auto itor = descriptorCount.find(type);
			if (itor == descriptorCount.end())
			{
				descriptorCount.emplace(type, 1);
			}
			else
			{
				itor->second++;
			}
		}
	}

	std::vector<VkDescriptorPoolSize> poolSizes{};
	VkDescriptorPoolSize poolSize{};
	for (const auto& kv : descriptorCount)
	{
		poolSize.type = kv.first;
		poolSize.descriptorCount = kv.second;
		poolSizes.emplace_back(poolSize);
	}

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = poolSizes.size();
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
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

void DescriptorPool::freeDescriptorSets(const std::vector<VkDescriptorSet>& descriptorSets)
{
	VK_CHECK(vkFreeDescriptorSets(VulkanContext::getDevice()->getVkDevice(), m_vkDescriptorPool, descriptorSets.size(),descriptorSets.data()));
}
