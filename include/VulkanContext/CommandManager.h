#pragma once

#include <list>

#include <vulkan/vulkan.h>

class Command;
class Mesh;

class CommandManager
{
public:
	static CommandManager* instance();

	void generateCommands();

	void generateCommands(Mesh* mesh);

	void cleanCommands();

	void destroy();

	void record(VkCommandBuffer commandBuffer);

private:
	~CommandManager();

	static CommandManager* g_instance;

	std::list< Command*> m_commands;
};

