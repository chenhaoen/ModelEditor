#pragma once

#include <cstdint>
#include <vector>

class Frame;
class FrameManager
{
public:
    static FrameManager* instance();

    void frame();

    void destroy();

    static uint32_t getMaxFrameCount()
    {
        return 2;
    }
private:
    FrameManager();
    ~FrameManager();

    static FrameManager* g_instance;

    std::vector<Frame *> m_frames;
    uint32_t m_currentFrame;
};