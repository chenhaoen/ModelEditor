#pragma once

#include "Core/Commands/Command.h"

class BindDescriptorSetsCommand :
    public Command
{
public:
    BindDescriptorSetsCommand();

    void record() override final;
};

