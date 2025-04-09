#pragma once

#include "Core/Exports.h"

class CORE_API InitInfo
{
public:
	InitInfo();

	bool m_debug;

	void* m_platformWindow;
};

