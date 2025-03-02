#pragma once

#include <glm/glm.hpp>

#include "RenderingContextDriver/Exports.h"

class RENDERINGCONTEXTDRIVER_API InitInfo
{
public:
	InitInfo();

	bool m_debug;

	void* m_platformWindow;
};

