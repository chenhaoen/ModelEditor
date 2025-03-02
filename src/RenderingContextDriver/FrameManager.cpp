#include <algorithm>
#include <memory>

#include "RenderingContextDriver/FrameManager.h"
#include "RenderingContextDriver/Frame.h"

FrameManager* FrameManager::g_instance = nullptr;

FrameManager* FrameManager::instance()
{
	if (g_instance == nullptr)
	{
		g_instance = new FrameManager();
	}

	return g_instance;
}

void FrameManager::destroy()
{
	delete g_instance;
	g_instance = nullptr;
}

FrameManager::FrameManager()
	:m_currentFrame(0)
{
	for (uint32_t i = 0; i < maxFrameCount(); ++i)
	{
		m_frames.emplace_back(new Frame());
	}
}

FrameManager::~FrameManager()
{
	std::for_each(m_frames.begin(), m_frames.end(), std::default_delete<Frame>());
}

void FrameManager::beginFrame()
{
	m_frames[m_currentFrame]->begin();
	
}

void FrameManager::endFrame()
{
	m_frames[m_currentFrame]->end();
	m_currentFrame = (m_currentFrame + 1) % maxFrameCount();
}

CommandBufferID FrameManager::currentCommandBuffer()
{
	return m_frames[m_currentFrame]->getCommandBuffer();
}
