#include <algorithm>
#include <memory>

#include "VulkanContext/CommandManager.h"
#include "VulkanContext/DrawCommand.h"
#include "VulkanContext/PipelineCommand.h"
#include "VulkanContext/PipelineManager.h"
#include "VulkanContext/Utils.h"

#include "Nodes/NodeManager.h"
#include "Nodes/Mesh.h"

CommandManager* CommandManager::g_instance = nullptr;

CommandManager* CommandManager::instance()
{
	if (!g_instance)
	{
		g_instance = new CommandManager();
	}

	return g_instance;
}

void CommandManager::generateCommands()
{
	if (!NodeManager::root()->isChanged())
	{
		return;
	}

	cleanCommands();

	m_commands.push_back(new PipelineCommand(PipelineManager::instance()->getPipeline(g_modelPipelineName)));
	m_commands.push_back(new DrawCommand(3, 1, 0, 0));
}

void CommandManager::cleanCommands()
{
	std::for_each(m_commands.begin(), m_commands.end(), std::default_delete<Command>());
	m_commands.clear();
}

void CommandManager::destroy()
{
	delete g_instance;
	g_instance = nullptr;
}

void CommandManager::record(VkCommandBuffer commandBuffer)
{
	generateCommands();

	for (Command* command : m_commands)
	{
		command->record(commandBuffer);
	}
}

CommandManager::~CommandManager()
{
	cleanCommands();
}
