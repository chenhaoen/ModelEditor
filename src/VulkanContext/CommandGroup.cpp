#include <algorithm>
#include <memory>

#include "CommandGroup.h"

CommandGroup::~CommandGroup()
{
	std::for_each(m_commands.begin(), m_commands.end(), std::default_delete<Command>());
}

void CommandGroup::addCommand(Command* command)
{
	m_commands.emplace_back(command);
}

void CommandGroup::record(VkCommandBuffer commandBuffer)
{
	for (Command* command : m_commands)
	{
		command->record(commandBuffer);
	}
}