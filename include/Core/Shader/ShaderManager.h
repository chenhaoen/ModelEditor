#pragma once

#include "Core/Exports.h"

class Shader;

class CORE_API ShaderManager
{
public:
	static ShaderManager* instance();

	void init();

	std::shared_ptr<Shader> getShader(const std::string_view& shaderName);
private:
	ShaderManager();
	~ShaderManager();

	static ShaderManager* g_instance;

	std::unordered_map<std::string_view, std::shared_ptr<Shader>> m_shaders;
};

