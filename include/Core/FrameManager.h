#pragma once

#include <vector>
#include <memory>

#include "Core/Exports.h"

#include "Core/RenderingContextDriver/Commons.h"

class Frame;
class CommandGroup;
class CORE_API FrameManager
{
public:
	static FrameManager* instance();
	static void destroy();

	static constexpr int maxFrameCount() { return 2; };

	uint32_t currentFrameIndex() const { return m_currentFrame; };

	void beginFrame();
	void endFrame();

	CommandBufferID currentCommandBuffer();

	std::shared_ptr<CommandGroup> currentCommandGroup();
private:
	FrameManager();
	~FrameManager();

	static FrameManager* g_instance;

	std::vector<Frame*> m_frames;

	uint32_t m_currentFrame;
};

