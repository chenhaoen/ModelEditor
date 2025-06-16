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

	void setUniformType(UniformType uniformType);
	UniformType getUniformType() const;

	void setShaderStageFlags(ShaderStageFlags);
	ShaderStageFlags getShaderStageFlags() const;
private:
	uint32_t m_binding;

	UniformType m_uniformType;

	ShaderStageFlags m_shaderStageFlags;
};

