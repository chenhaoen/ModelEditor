#pragma once

#include <vector>

#include "RenderingContextDriver/Exports.h"

class Frame;
class RENDERINGCONTEXTDRIVER_API FrameManager
{
public:
	static FrameManager* instance();
	static void destroy();

	static constexpr int maxFrameCount() { return 2; };

	void beginFrame();
	void endFrame();

private:
	FrameManager();
	~FrameManager();

	static FrameManager* g_instance;

	std::vector<Frame*> m_frames;

	uint32_t m_currentFrame;
};

