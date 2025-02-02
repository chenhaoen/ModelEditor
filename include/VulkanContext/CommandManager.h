#pragma once

#include <list>

#include <vulkan/vulkan.h>

class Command;
class CommandManager
{
public:
	static CommandManager* instance();

	void generateCommands();

	void cleanCommands();

	void destroy();

	void record(VkCommandBuffer commandBuffer);

private:
	~CommandManager();

	static CommandManager* g_instance;

	std::list< Command*> m_commands;
};

