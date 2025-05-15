#include "Core/Shader/Shader.h"

#include "Core/RenderingContextDriver/RenderingContextDriver.h"

Shader::Shader()
	:m_type(ShaderType::Vertex)
{
}

void Shader::setName(const std::string_view& name)
{
	m_name = name;
}

const std::string_view& Shader::getName() const
{
	return m_name;
}

void Shader::setFileName(const std::string_view& fileName)
{
	m_fileName = fileName;
}

const std::string_view& Shader::getFileName() const
{
	return m_fileName;
}

void Shader::setType(const ShaderType type)
{
	m_type = type;
}

ShaderType Shader::getType() const
{
	return m_type;
}

void Shader::setfuncName(const std::string_view& funcName)
{
	m_funcName = funcName;
}

const std::string_view& Shader::getFuncName() const
{
	return m_funcName;
}
