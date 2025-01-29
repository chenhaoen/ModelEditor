#pragma once

#include <vector>

#include "VulkanContext/Exports.h"
#include "VulkanContext/InitInfo.h"

class Instance;

class VULKANCONTEXT_API VulkanContext
{
public:
    static VulkanContext *instance();

    void setInitInfo(const InitInfo& initInfo);
    const InitInfo& getInitInfo() const;

    void init();

    void destory();

    void drawFrame();

    const std::vector<const char*>& getInstanceExtensions() const;
    const std::vector<const char*>& getDeviceExtensions() const;
    const std::vector<const char*>& getInstanceLayers() const;
private:
    VulkanContext();
    ~VulkanContext();

    static std::vector<const char*> getRequiredInstanceExtensions();

private:

    static VulkanContext *g_instance;

    InitInfo m_initInfo;

    std::vector<const char*> m_instanceExtensions;
    std::vector<const char*> m_instanceLayers;
    std::vector<const char*> m_deviceExtensions;

    Instance* m_instance;
};