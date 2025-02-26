#pragma once

#include "RenderingContextDriver/Commons.h"

class Frame
{
public:
	Frame();
	~Frame();

	CommandBufferID getCommandBuffer() const;

	void begin();
	void end();
private:
	SemaphoreID m_imageAvailableSemaphore;
	SemaphoreID m_renderFinishedSemaphore;
	FenceID m_inFlightFence;

	CommandBufferID m_commandBuffer;

	uint32_t m_imageIndex;
};

