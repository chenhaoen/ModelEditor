#pragma once

#include "Core/Commands/Command.h"

class BindPipelineCommand :
    public Command
{
public:
    BindPipelineCommand();

    void record() override final;

private:
};

