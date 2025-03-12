#include "VulkanContext/CommandPool.h"
#include "VulkanContext/LogicalDevice.h"
#include "VulkanContext/PhysicalDevice.h"
#include "VulkanContext/VulkanContext.h"
#include "VulkanContext/Utils.h"

CommandPool::CommandPool()
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = VulkanContext::getPhysicalDevice()->getGraphicsFamilyIndex();

    VK_CHECK(vkCreateCommandPool(VulkanContext::getDevice()->getVkDevice(), &poolInfo, nullptr, &m_vkCommandPool));
}

CommandPool::~CommandPool()
{
    vkDestroyCommandPool(VulkanContext::getDevice()->getVkDevice(), m_vkCommandPool, nullptr);
}

VkCommandPool CommandPool::getVkCommandPool() const
{
    return m_vkCommandPool;
}

VkCommandBuffer CommandPool::createCommands()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_vkCommandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(VulkanContext::getDevice()->getVkDevice(), &allocInfo, &commandBuffer);

    return commandBuffer;
}

VkCommandBuffer CommandPool::beginSingleTimeCommands()
{
    VkCommandBuffer commandBuffer = createCommands();

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo));

    return commandBuffer;
}

void CommandPool::endSingleTimeCommands(VkCommandBuffer commandBuffer)
{
    VK_CHECK(vkEndCommandBuffer(commandBuffer));

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    VK_CHECK(vkQueueSubmit(VulkanContext::getDevice()->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE));
    VK_CHECK(vkQueueWaitIdle(VulkanContext::getDevice()->getGraphicsQueue()));

    vkFreeCommandBuffers(VulkanContext::getDevice()->getVkDevice(), m_vkCommandPool, 1, &commandBuffer);
}