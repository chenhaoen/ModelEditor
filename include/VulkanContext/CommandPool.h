#pragma once

#include <vulkan/vulkan.h>

class CommandPool
{
public:
    CommandPool();
    ~CommandPool();

    VkCommandPool getVkCommandPool() const;

    VkCommandBuffer createCommands();

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
private:
    VkCommandPool m_vkCommandPool;
};
