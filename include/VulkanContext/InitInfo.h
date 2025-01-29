#pragma once

#include "VulkanContext/Exports.h"

class VULKANCONTEXT_API InitInfo
{
public:
	InitInfo();

	void* m_platformWindow;

	bool m_debug;
};

