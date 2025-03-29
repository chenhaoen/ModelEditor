#pragma once

#include "Core/Commands/Command.h"
#include "Core/Exports.h"

#include "RenderingContextDriver/Commons.h"

class CORE_API SetScissorCommand :
    public Command
{
public:
    SetScissorCommand(const Rect2D&);

    void record() override final;

private:
    Rect2D m_scissor;
};

