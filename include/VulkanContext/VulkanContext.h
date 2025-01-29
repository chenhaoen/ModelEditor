#pragma once

#include "VulkanContext/Exports.h"
#include "VulkanContext/InitInfo.h"

class VULKANCONTEXT_API VulkanContext
{
public:
    static VulkanContext *instance();

    void init();
    
    void setInitInfo(const InitInfo& initInfo );

    void destory();

    void drawFrame();

private:
    VulkanContext();
    ~VulkanContext();

    static VulkanContext *g_instance;

    void* m_platformWindow;

    InitInfo m_initInfo;
};