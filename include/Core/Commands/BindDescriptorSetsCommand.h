#pragma once

#include "Core/Commands/Command.h"

#include "Core/RenderingContextDriver/Commons.h"

class BindDescriptorSetsCommand :
    public Command
{
public:
    BindDescriptorSetsCommand(PipelineName);

    void record(CommandBufferID) override final;

private:
    PipelineName m_name;
};

