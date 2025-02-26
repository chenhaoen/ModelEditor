#pragma once

#include "VulkanContext/Command.h"

class SetViewportCommand :
    public Command
{
public:
    SetViewportCommand(const VkViewport& viewport);

    void record(VkCommandBuffer commandBuffer) override;

private:
    VkViewport m_viewport;
};

