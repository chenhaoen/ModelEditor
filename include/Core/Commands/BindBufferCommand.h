#pragma once

#include "Core/Commands/Command.h"

#include "RenderingContextDriver/Commons.h"

class BindBufferCommand :
    public Command
{
public:
    BindBufferCommand(BufferID buffer, BufferType type);
    
    void record() override final;
private:
    BufferID m_buffer;
    BufferType m_bufferType;
};

