#include "VulkanContext/CommandManager.h"
#include "VulkanContext/Command.h"

CommandManager* CommandManager::g_instance = nullptr;

CommandManager* CommandManager::instance()
{
	if (!g_instance)
	{
		g_instance = new CommandManager();
	}

	return g_instance;
}

void CommandManager::destroy()
{
	delete g_instance;
	g_instance = nullptr;
}

void CommandManager::record(VkCommandBuffer commandBuffer)
{
	for (Command* command : m_commands)
	{
		command->record(commandBuffer);
	}
}
