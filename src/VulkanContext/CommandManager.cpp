#include <algorithm>
#include <memory>

#include "VulkanContext/CommandManager.h"
#include "VulkanContext/DrawCommand.h"
#include "VulkanContext/PipelineCommand.h"
#include "VulkanContext/SetScissorCommand.h"
#include "VulkanContext/SetViewportCommand.h"
#include "VulkanContext/PipelineManager.h"
#include "VulkanContext/Utils.h"
#include "VulkanContext/VulkanContext.h"
#include "VulkanContext/Surface.h"

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

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)VulkanContext::getSurface()->getExtent().width;
	viewport.height = (float)VulkanContext::getSurface()->getExtent().height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	m_commands.push_back(new SetViewportCommand(viewport));

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = VulkanContext::getSurface()->getExtent();

	m_commands.push_back(new SetScissorCommand(scissor));

	//m_commands.push_back(new DrawCommand(3, 1, 0, 0));
}

void CommandManager::generateCommands(Mesh* mesh)
{
	mesh;
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
