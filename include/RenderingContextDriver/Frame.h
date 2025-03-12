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

	void addBoundUniform(const BoundUniform& boundUniform);

	void compile();
private:
	void updateUniformBuffer();

private:
	SemaphoreID m_imageAvailableSemaphore;
	SemaphoreID m_renderFinishedSemaphore;
	FenceID m_inFlightFence;

	CommandBufferID m_commandBuffer;
	UniformSetID m_uniformSet;

	uint32_t m_imageIndex;

	std::vector<BoundUniform> m_boundUniforms;

	bool m_compiled;
};

