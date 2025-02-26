#pragma once

#include "VulkanContext/Command.h"

class SetScissorCommand :
    public Command
{
public:
    SetScissorCommand(const VkRect2D& scissor);

    void record(VkCommandBuffer commandBuffer) override;

private:
    VkRect2D m_scissor;
};

