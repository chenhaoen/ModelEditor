#include <iostream>

#include <vulkan/vulkan.h>

#ifdef _WIN64
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#endif // _Win64

#include "VulkanContext/VulkanContext.h"
#include "VulkanContext/Instance.h"
#include "VulkanContext/DebugUtilsMessenger.h"
#include "VulkanContext/LogicalDevice.h"
#include "VulkanContext/PhysicalDevice.h"
#include "VulkanContext/Surface.h"
#include "VulkanContext/RenderPass.h"
#include "VulkanContext/SwapChain.h"
#include "VulkanContext/CommandPool.h"
#include "VulkanContext/FrameManager.h"
#include "VulkanContext/CommandManager.h"

VulkanContext *VulkanContext::g_instance = nullptr;

VulkanContext::VulkanContext()
    :m_instance(nullptr)
    , m_debugUtilsMessenger(nullptr)
    , m_physicalDevice(nullptr)
    , m_surface(nullptr)
    , m_device(nullptr)
    , m_renderPass(nullptr)
    , m_swapChain(nullptr)
    , m_commandPool(nullptr)
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

    if (m_initInfo.m_debug)
    {
        m_debugUtilsMessenger = new DebugUtilsMessenger();
    }

    m_physicalDevice = m_instance->getBestPhysicalDevice();
    if (m_physicalDevice == nullptr)
    {
        throw std::runtime_error("failed to find a suitable GPU!");
    }

    m_surface = new Surface(m_initInfo.m_platformWindow);
    
    m_device = new LogicalDevice(m_physicalDevice);

    m_renderPass = new RenderPass();

    m_swapChain = new SwapChain();

    m_commandPool = new CommandPool();

    FrameManager::instance();
    CommandManager::instance();
}

void VulkanContext::setInitInfo(const InitInfo& initInfo)
{
    m_initInfo = initInfo;
}

const InitInfo& VulkanContext::getInitInfo() const
{
    return m_initInfo;
}

void VulkanContext::destroy()
{
    m_device->Wait();

    FrameManager::instance()->destroy();
    CommandManager::instance()->destroy();

    delete m_commandPool;
    delete m_swapChain;
    delete m_renderPass;
    delete m_device;
    delete m_surface;
    delete m_physicalDevice;
    delete m_debugUtilsMessenger;
    delete m_instance;
}

void VulkanContext::drawFrame()
{
    FrameManager::instance()->frame();
}

void VulkanContext::resizeSurface()
{
    if (m_swapChain == nullptr)
    {
        return;
    }

    m_device->Wait();
    m_swapChain->cleanup();
    m_swapChain->create();
    m_swapChain->createImageViews();
    m_swapChain->createFrameBuffers();
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
