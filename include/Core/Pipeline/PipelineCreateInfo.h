#pragma once

#include "Core/RenderingContextDriver/Commons.h"

#include "Core/Exports.h"
#include "Core/DynamicState.h"

class Shader;
class CORE_API PipelineCreateInfo
{
public:
	PipelineCreateInfo();

	RenderPrimitive m_renderPrimitive;

	std::list<DynamicStateType> m_dynamicStates;

	std::list<std::shared_ptr<Shader>> m_shaders;

	PipelineType m_type;

	PipelineRasterizationState m_rasterizationState;

	PipelineMultisampleState m_multrsampleState;
};
