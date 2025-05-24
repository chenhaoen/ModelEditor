#pragma once

#include "Core/Commands/Command.h"

#include "Core/RenderingContextDriver/Commons.h"

class DrawCommand final :
    public Command
{
public:
    DrawCommand(
        DrawMode mode,
        uint32_t indexCount, 
        uint32_t instanceCount,
        uint32_t firstIndex, 
        int32_t vertexOffset, 
        uint32_t firstInstance);
    ~DrawCommand() override final;

    void record(CommandBufferID) override final;

private:
    DrawMode m_mode;

    uint32_t m_indexCount;
    uint32_t m_instanceCount;
    uint32_t m_firstIndex;
    int32_t m_vertexOffset;
    uint32_t m_firstInstance;
};

