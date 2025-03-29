#include "Core/Commands/CommandGroup.h"

void CommandGroup::push(std::shared_ptr<Command> command)
{
	m_commands.push_back(command);
}

void CommandGroup::clear()
{
	m_commands.clear();
}

void CommandGroup::record()
{
	for (auto command : m_commands)
	{
		command->record();
	}
}
