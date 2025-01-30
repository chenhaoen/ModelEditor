#pragma once

#include <vector>

#include "VulkanContext/Exports.h"
#include "VulkanContext/InitInfo.h"

class Instance;
class DebugUtilsMessenger;
class PhysicalDevice;
class Surface;
class LogicalDevice;
class RenderPass;
class SwapChain;
class DescriptorSetLayout;
class Pipeline;

class VULKANCONTEXT_API VulkanContext
{
public:
	static VulkanContext* instance();

	void setInitInfo(const InitInfo& initInfo);
	const InitInfo& getInitInfo() const;

	void init();

	void destroy();

	void drawFrame();

	const std::vector<const char*>& getInstanceExtensions() const;
	const std::vector<const char*>& getDeviceExtensions() const;
	const std::vector<const char*>& getInstanceLayers() const;

	Instance* getVulkanInstance()
	{
		return m_instance;
	}

	PhysicalDevice* getPhysicalDevice()
	{
		return m_physicalDevice;
	}

	Surface* getSurface()
	{
		return m_surface;
	}

	LogicalDevice* getDevice()
	{
		return m_device;
	}

	RenderPass* getRenderPass()
	{
		return m_renderPass;
	}

	SwapChain* getSwapChain()
	{
		return m_swapChain;
	}

	Pipeline* getPipeline()
	{
		return m_pipeLine;
	}

	static uint32_t getMaxFrameCount()
	{
		return 2;
	}

private:
	VulkanContext();
	~VulkanContext();

	static std::vector<const char*> getRequiredInstanceExtensions();

private:

	static VulkanContext* g_instance;

	InitInfo m_initInfo;

	std::vector<const char*> m_instanceExtensions;
	std::vector<const char*> m_instanceLayers;
	std::vector<const char*> m_deviceExtensions;

	Instance* m_instance;
	DebugUtilsMessenger* m_debugUtilsMessenger;
	PhysicalDevice* m_physicalDevice;
	Surface* m_surface;
	LogicalDevice* m_device;
	RenderPass* m_renderPass;
	SwapChain* m_swapChain;
	DescriptorSetLayout* m_descriptorSetLayout;
	Pipeline* m_pipeLine;
};