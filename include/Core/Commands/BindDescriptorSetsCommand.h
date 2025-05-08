#pragma once

#include "Core/Commands/Command.h"

#include "Core/RenderingContextDriver/Commons.h"

class BindDescriptorSetsCommand :
    public Command
{
public:
    BindDescriptorSetsCommand(PipelineType type);

    void record() override final;

private:
    PipelineType m_type;
};

