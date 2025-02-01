#include <algorithm>
#include <memory>

#include "VulkanContext/FrameManager.h"
#include "VulkanContext/VulkanContext.h"
#include "VulkanContext/Frame.h"
#include "VulkanContext/CommandManager.h"

FrameManager* FrameManager::g_instance = nullptr;

FrameManager::FrameManager()
    : m_currentFrame(0)
{
    for (uint32_t i = 0; i < getMaxFrameCount(); ++i)
    {
        m_frames.push_back(new Frame(i));
    }
}

FrameManager::~FrameManager()
{
    std::for_each(m_frames.begin(), m_frames.end(), std::default_delete<Frame>());
}

FrameManager* FrameManager::instance()
{
    if (!g_instance)
    {
        g_instance = new FrameManager();
    }

    return g_instance;
}

void FrameManager::frame()
{
    Frame *currentFrame = m_frames[m_currentFrame];
    currentFrame->begin();

    CommandManager::instance()->record(currentFrame->getCommandBuffer());

    currentFrame->end();

    m_currentFrame = (m_currentFrame + 1) % getMaxFrameCount();
}

void FrameManager::destroy()
{
    delete g_instance;
    g_instance = nullptr;
}
