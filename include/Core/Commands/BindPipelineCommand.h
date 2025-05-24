#pragma once

#include "Core/Commands/Command.h"

#include "Core/RenderingContextDriver/Commons.h"

class BindPipelineCommand :
    public Command
{
public:
    BindPipelineCommand(PipelineName name);

    void record(CommandBufferID) override final;

private:
    PipelineName m_name;
};

