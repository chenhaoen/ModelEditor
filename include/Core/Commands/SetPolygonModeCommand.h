#pragma once

#include "Core/Commands/Command.h"

#include "RenderingContextDriver/Commons.h"

class SetPolygonModeCommand :
    public Command
{
public:
    SetPolygonModeCommand(FillMode fillMode);

    void record() override final;
private:
    FillMode m_fillMode;
};

