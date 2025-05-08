#pragma once

#include "Core/Commands/Command.h"

#include "Core/RenderingContextDriver/Commons.h"

class BindPipelineCommand :
    public Command
{
public:
    BindPipelineCommand(PipelineType type);

    void record() override final;

private:
    PipelineType m_type;
};

