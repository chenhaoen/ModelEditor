#pragma once

#include <list>

#include <vulkan/vulkan.h>

class Command;
class CommandManager
{
public:
	static CommandManager* instance();

	void destroy();

	void record(VkCommandBuffer commandBuffer);

private:
	static CommandManager* g_instance;

	std::list< Command*> m_commands;
};

