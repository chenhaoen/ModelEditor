#include "VulkanContext/Surface.h"
#include "VulkanContext/Instance.h"
#include "VulkanContext/VulkanContext.h"
#include "VulkanContext/Utils.h"

#ifdef _WIN64
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#endif // _WIN64

Surface::Surface(void* platformWindow)
{
#ifdef _WIN64
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.hinstance = (HINSTANCE)GetModuleHandle(NULL);
	surfaceCreateInfo.hwnd = (HWND)platformWindow;
	VK_CHECK(vkCreateWin32SurfaceKHR(VulkanContext::instance()->getVulkanInstance()->getVkInstance(), &surfaceCreateInfo, nullptr, &m_vkSurface));
#endif // _WIN64

}

Surface::~Surface()
{
	vkDestroySurfaceKHR(VulkanContext::instance()->getVulkanInstance()->getVkInstance(), m_vkSurface, nullptr);
}

VkSurfaceKHR Surface::getVkSurface() const
{
	return m_vkSurface;
}
