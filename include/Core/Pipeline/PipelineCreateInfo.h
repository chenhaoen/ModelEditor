#pragma once

#include "Core/RenderingContextDriver/Commons.h"

#include "Core/Exports.h"
#include "Core/DynamicState.h"

class CORE_API PipelineCreateInfo
{
public:
	PipelineCreateInfo();

	RenderPrimitive m_renderPrimitive;

	std::list<DynamicStateType> m_dynamicStates;
};
