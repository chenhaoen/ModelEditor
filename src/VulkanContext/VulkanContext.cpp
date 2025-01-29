#include <vulkan/vulkan.h>

#ifdef _WIN64
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#endif // _Win64

#include "VulkanContext/VulkanContext.h"
#include "VulkanContext/Instance.h"

VulkanContext *VulkanContext::g_instance = nullptr;

VulkanContext::VulkanContext()
    :m_instance(nullptr)
{
}

VulkanContext::~VulkanContext()
{
}

std::vector<const char*> VulkanContext::getRequiredInstanceExtensions()
{
    std::vector<const char*> result;

#ifdef _WIN64
    result.emplace_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif // _WIN64

    result.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);

    return result;
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
    const std::vector<const char*>& windowInstanceExtensions = getRequiredInstanceExtensions();
    m_instanceExtensions.insert(m_instanceExtensions.end(), windowInstanceExtensions.begin(), windowInstanceExtensions.end());
    m_deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    if (m_initInfo.m_debug)
    {
        m_instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        m_instanceLayers.push_back("VK_LAYER_KHRONOS_validation");
    }

    m_instance = new Instance();
}

void VulkanContext::setInitInfo(const InitInfo& initInfo)
{
    m_initInfo = initInfo;
}

const InitInfo& VulkanContext::getInitInfo() const
{
    return m_initInfo;
}

void VulkanContext::destory()
{
    delete m_instance;
}

void VulkanContext::drawFrame()
{
}

const std::vector<const char*>& VulkanContext::getInstanceExtensions() const
{
    return m_instanceExtensions;
}

const std::vector<const char*>& VulkanContext::getDeviceExtensions() const
{
    return m_deviceExtensions;
}

const std::vector<const char*>& VulkanContext::getInstanceLayers() const
{
    return m_instanceLayers;
}
