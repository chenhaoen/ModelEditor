#include "VulkanContext/VulkanContext.h"

VulkanContext *VulkanContext::g_instance = nullptr;

VulkanContext::VulkanContext()
{
}

VulkanContext::~VulkanContext()
{
}

VulkanContext *VulkanContext::instance()
{
    if (g_instance == nullptr)
    {
        g_instance = new VulkanContext();
    }

    return g_instance;
}

void VulkanContext::init()
{
    
}

void VulkanContext::setInitInfo(const InitInfo& initInfo)
{
    m_initInfo = initInfo;
}

void VulkanContext::destory()
{
}

void VulkanContext::drawFrame()
{
}