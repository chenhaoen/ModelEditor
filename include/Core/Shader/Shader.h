#pragma once

#include "Core/RenderingContextDriver/Commons.h"

#include "Core/Exports.h"

enum class ShaderType
{
	Vertex,
	Fragment,
	Compute
};

class Descriptor;

class CORE_API Shader
{
public:
	Shader();
	~Shader() = default;

	void setName(const std::string_view& name);
	const std::string_view& getName() const;

	void setFileName(const std::string_view& fileName);
	const std::string_view& getFileName() const;

	void setType(const ShaderType type);
	ShaderType getType() const;

	void setfuncName(const std::string_view& funcName);
	const std::string_view& getFuncName() const;

	const std::list<std::shared_ptr<Descriptor>>& getDescriptors();
	void addDescriptor(std::shared_ptr<Descriptor>);
private:
	std::string_view m_fileName;

	std::string_view m_name;

	std::string_view m_funcName;

	ShaderType m_type;

	std::list<std::shared_ptr<Descriptor>> m_descriptors;
};

