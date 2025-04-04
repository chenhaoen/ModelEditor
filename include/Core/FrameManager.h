#pragma once

#include <vector>
#include <memory>

#include "Core/Exports.h"

#include "RenderingContextDriver/Commons.h"

class Frame;
class CommandGroup;
class CORE_API FrameManager
{
public:
	static FrameManager* instance();
	static void destroy();

	static constexpr int maxFrameCount() { return 2; };

	void beginFrame();
	void endFrame();

	CommandBufferID currentCommandBuffer();

	std::shared_ptr<CommandGroup> currentCommandGroup();

	UniformSetID currentUniformSet();
private:
	FrameManager();
	~FrameManager();

	static FrameManager* g_instance;

	std::vector<Frame*> m_frames;

	uint32_t m_currentFrame;
};

