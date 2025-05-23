#include "Core/Shader/Descriptor.h"

Descriptor::Descriptor()
	:m_binding(0)
	,m_uniformType(UniformType::UNIFORM_TYPE_UNIFORM_BUFFER)
{
}

Descriptor::~Descriptor()
{
}

void Descriptor::setBinding(uint32_t binding)
{
	m_binding = binding;
}

uint32_t Descriptor::getBinding() const
{
	return m_binding;
}

void Descriptor::setUniformType(UniformType uniformType)
{
	m_uniformType = uniformType;
}

UniformType Descriptor::getUniformType() const
{
	return m_uniformType;
}
