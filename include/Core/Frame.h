#pragma once

#include <map>
#include <memory>

#include "RenderingContextDriver/Commons.h"

class CommandGroup;
class Frame
{
public:
	Frame();
	~Frame();

	CommandBufferID getCommandBuffer() const;

	std::shared_ptr<CommandGroup> getCommandGroup() const;

	UniformSetID getUniformSet();

	void begin();
	void end();
private:
	SemaphoreID m_imageAvailableSemaphore;
	SemaphoreID m_renderFinishedSemaphore;
	FenceID m_inFlightFence;

	CommandBufferID m_commandBuffer;

	std::shared_ptr<CommandGroup> m_commandGroup;

	std::map<PipelineType,UniformSetID> m_descriptorSets;

	uint32_t m_imageIndex;
};

