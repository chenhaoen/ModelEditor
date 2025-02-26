#include "VulkanContext/BufferManager.h"

BufferManager* BufferManager::g_instance = nullptr;

BufferManager* BufferManager::instance()
{
    if (!g_instance)
    {
        g_instance = new BufferManager;
    }

    return g_instance;
}

void BufferManager::destory()
{
}

BufferManager::BufferManager()
{
}

BufferManager::~BufferManager()
{
}
