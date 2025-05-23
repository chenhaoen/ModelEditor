#pragma once

#include "Core/Commands/Command.h"
#include "Core/Exports.h"

class CORE_API CommandGroup :
    public Command
{
public:
    void push(std::shared_ptr<Command>);

    void clear();

    void record(CommandBufferID) override final;
private:
    std::vector<std::shared_ptr<Command>> m_commands;
};

