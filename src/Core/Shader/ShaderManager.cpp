#include "Core/Shader/ShaderManager.h"

ShaderManager* ShaderManager::g_instance = nullptr;

ShaderManager* ShaderManager::instance()
{
	if (!g_instance)
	{
		g_instance = new ShaderManager();
	}

	return g_instance;
}

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::init()
{
	// Model

}
