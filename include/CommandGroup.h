#pragma once

#include <vector>

#include "VulkanContext/Command.h"

class CommandGroup : public Command
{
public:
	CommandGroup() = default;
	~CommandGroup();

	void addCommand(Command* command);

	void record(VkCommandBuffer commandBuffer);
private:
	std::vector<Command*> m_commands;
};

