#pragma once

#include "Core/Commands/Command.h"
#include "Core/Exports.h"

#include "Core/RenderingContextDriver/Commons.h"

class CORE_API SetViewportCommand :
    public Command
{
public:
    SetViewportCommand(const Viewport&);

    void record(CommandBufferID) override final;

private:
    Viewport m_viewport;
};

