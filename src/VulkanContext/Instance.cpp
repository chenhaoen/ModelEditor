#include "VulkanContext/Instance.h"
#include "VulkanContext/VulkanContext.h"
#include "VulkanContext/Utils.h"
#include "VulkanContext/DebugUtilsMessenger.h"
#include "VulkanContext/PhysicalDevice.h"

Instance::Instance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Application";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "3D Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_HEADER_VERSION_COMPLETE;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    createInfo.enabledExtensionCount = VulkanContext::instance()->getInstanceExtensions().size();
    createInfo.ppEnabledExtensionNames = VulkanContext::instance()->getInstanceExtensions().data();

    createInfo.enabledLayerCount = VulkanContext::instance()->getInstanceLayers().size();
    createInfo.ppEnabledLayerNames = VulkanContext::instance()->getInstanceLayers().data();

    if (VulkanContext::instance()->getInitInfo().m_debug)
    {
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        DebugUtilsMessenger::populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }

    VK_CHECK(vkCreateInstance(&createInfo, nullptr, &m_vkInstance));

    {
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        m_extensionProperties.resize(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, m_extensionProperties.data());
    }

    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        m_availableLayers.resize(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, m_availableLayers.data());
    }
}

Instance::~Instance()
{
    vkDestroyInstance(m_vkInstance, nullptr);
}

VkInstance Instance::getVkInstance() const
{
    return m_vkInstance;
}

PhysicalDevice* Instance::getBestPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, nullptr);

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, devices.data());

    if (deviceCount == 0)
    {
        return nullptr;
    }

    VkPhysicalDevice bestPhysicalDevice = devices.front();

    return new PhysicalDevice(bestPhysicalDevice);
}
