#pragma once

#include "Core/Commands/Command.h"

#include "Core/RenderingContextDriver/Commons.h"

class SetPolygonModeCommand :
    public Command
{
public:
    SetPolygonModeCommand(FillMode fillMode);

    void record(CommandBufferID) override final;
private:
    FillMode m_fillMode;
};

