#pragma once

#include "Core/Shader/Shader.h"
#include "Core/Exports.h"

class CORE_API Descriptor
{
public:
	Descriptor();
	~Descriptor();

	void setBinding(uint32_t binding);
	uint32_t getBinding() const;

	void setShaderType(ShaderType shaderType);
	ShaderType getShaderType() const;

	void setUniformType(UniformType uniformType);
	UniformType getUniformType() const;
private:
	uint32_t m_binding;

	ShaderType m_shaderType;

	UniformType m_uniformType;
};

