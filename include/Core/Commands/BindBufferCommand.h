#pragma once

#include "Core/Commands/Command.h"

#include "Core/RenderingContextDriver/Commons.h"

class BindBufferCommand :
    public Command
{
public:
    BindBufferCommand(BufferID buffer, BufferType type);
    
    void record(CommandBufferID) override final;
private:
    BufferID m_buffer;
    BufferType m_bufferType;
};

