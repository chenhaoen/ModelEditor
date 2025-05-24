#pragma once

#include "Core/RenderingContextDriver/Commons.h"

class Command
{
public:
	Command() = default;
	virtual ~Command() = 0;

	virtual void record(CommandBufferID) = 0;
};

