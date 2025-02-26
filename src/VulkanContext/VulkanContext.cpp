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
#include "VulkanContext/CommandManager.h"
#include "VulkanContext/PipelineManager.h"
#include "VulkanContext/Pipeline.h"
#include "VulkanContext/Utils.h"

#include "Nodes/NodeManager.h"
#include "Nodes/Mesh.h"

Instance* VulkanContext::m_instance = nullptr;
DebugUtilsMessenger* VulkanContext::m_debugUtilsMessenger = nullptr;
PhysicalDevice* VulkanContext::m_physicalDevice = nullptr;
Surface* VulkanContext::m_surface = nullptr;
LogicalDevice* VulkanContext::m_device = nullptr;
RenderPass* VulkanContext::m_renderPass = nullptr;
SwapChain* VulkanContext::m_swapChain = nullptr;
CommandPool* VulkanContext::m_commandPool = nullptr;
std::vector<const char*> VulkanContext::m_instanceExtensions;
std::vector<const char*> VulkanContext::m_instanceLayers;
std::vector<const char*> VulkanContext::m_deviceExtensions;

VulkanContext::VulkanContext()
{
    
}

VulkanContext::~VulkanContext()
{
    destroy();
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

void VulkanContext::initialize()
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
}

void VulkanContext::destroy()
{
    m_device->wait();

    PipelineManager::instance()->destroy();
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

void VulkanContext::wait()
{
    m_device->wait();
}

void VulkanContext::draw()
{
}

void VulkanContext::resizeSurface()
{
    if (m_swapChain == nullptr)
    {
        return;
    }

    m_device->wait();
    m_swapChain->cleanup();
    m_swapChain->create();
    m_swapChain->createImageViews();
    m_swapChain->createFrameBuffers();

    NodeManager::root()->setChanged(true);
}

CommandBufferID VulkanContext::createCommandBuffer()
{
    return CommandBufferID(m_commandPool->createCommands());
}

void VulkanContext::resetCommandBuffer(CommandBufferID commandBuffer)
{
    VK_CHECK(vkResetCommandBuffer(reinterpret_cast<VkCommandBuffer>(commandBuffer.id), 0));
}

void VulkanContext::beginCommandBuffer(CommandBufferID commandBuffer)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;                  // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    VK_CHECK(vkBeginCommandBuffer(reinterpret_cast<VkCommandBuffer>(commandBuffer.id), &beginInfo));
}

void VulkanContext::endCommandBuffer(CommandBufferID commandBuffer)
{
    VK_CHECK(vkEndCommandBuffer(reinterpret_cast<VkCommandBuffer>(commandBuffer.id)));
}

FenceID VulkanContext::createFence()
{
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VkFence fence;
    VK_CHECK(vkCreateFence(VulkanContext::getDevice()->getVkDevice(), &fenceInfo, nullptr, &fence));

    return FenceID(fence);
}

void VulkanContext::waitFence(FenceID p_fence)
{
    VK_CHECK(vkWaitForFences(VulkanContext::getDevice()->getVkDevice(), 1, reinterpret_cast<VkFence*>(&p_fence.id), VK_TRUE, UINT64_MAX));

    VK_CHECK(vkResetFences(VulkanContext::getDevice()->getVkDevice(), 1, reinterpret_cast<VkFence*>(&p_fence.id)));
}

void VulkanContext::freeFence(FenceID p_fence)
{
    vkDestroyFence(VulkanContext::getDevice()->getVkDevice(), reinterpret_cast<VkFence>(p_fence.id), nullptr);
}

SemaphoreID VulkanContext::createSemaphore()
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkSemaphore semaphore;
    VK_CHECK(vkCreateSemaphore(VulkanContext::getDevice()->getVkDevice(), &semaphoreInfo, nullptr, &semaphore));

    return SemaphoreID(semaphore);
}

void VulkanContext::freeSemaphore(SemaphoreID p_semaphore)
{
    vkDestroySemaphore(VulkanContext::getDevice()->getVkDevice(), reinterpret_cast<VkSemaphore>(p_semaphore.id), nullptr);
}

uint32_t VulkanContext::getNextImageIndex(SemaphoreID semaphore)
{
    return m_swapChain->getNextImageIndex(reinterpret_cast<VkSemaphore>(semaphore.id));
}

SurfaceID VulkanContext::getSurfaceID()
{
    return SurfaceID(m_surface);
}

Extent2D VulkanContext::getSurfaceExtent(SurfaceID)
{
    Extent2D result;
    result.width = m_surface->getExtent().width;
    result.height = m_surface->getExtent().height;
    return result;
}

RenderPassID VulkanContext::getRenderPassID()
{
    return RenderPassID(m_renderPass);
}

void VulkanContext::beginRenderPass(CommandBufferID commandBuffer, FramebufferID frameBuffer)
{
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = VulkanContext::getRenderPass()->getVkRenderPass();
    renderPassInfo.framebuffer = reinterpret_cast<VkFramebuffer>(frameBuffer.id);

    std::array<VkClearValue, 2> clearValues{};
    
    clearValues[0] = {0.0f, 0.0f, 0.0f ,1.0f};
    clearValues[1] = { 1.0f, 0 };
    renderPassInfo.clearValueCount = clearValues.size();
    renderPassInfo.pClearValues = clearValues.data();

    renderPassInfo.renderArea.extent = m_surface->getExtent();

    vkCmdBeginRenderPass(reinterpret_cast<VkCommandBuffer>(commandBuffer.id), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanContext::endRenderPass(CommandBufferID commandBuffer)
{
    vkCmdEndRenderPass(reinterpret_cast<VkCommandBuffer>(commandBuffer.id));
}

SwapChainID VulkanContext::getSwapChainID()
{
    return SwapChainID(m_swapChain);
}

FramebufferID VulkanContext::getFramebuffer(SwapChainID swapChain, uint32_t imageIndex)
{ 
    return FramebufferID(reinterpret_cast<SwapChain*>(swapChain.id)->getFrameBuffer(imageIndex));
}

void VulkanContext::queueSubmit(const std::vector<CommandBufferID>& inCommandBuffers, const std::vector<SemaphoreID>& inWaitSemaphore,
    const std::vector<SemaphoreID>& inSignalSemaphore, const FenceID& fence)
{
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    std::vector<VkSemaphore> waitSemaphores;
    for (SemaphoreID semaphore : inWaitSemaphore)
    {
        waitSemaphores.emplace_back(reinterpret_cast<VkSemaphore>(semaphore.id));
    }

    std::vector<VkSemaphore> signalSemaphores;
    for (SemaphoreID semaphore : inSignalSemaphore)
    {
        signalSemaphores.emplace_back(reinterpret_cast<VkSemaphore>(semaphore.id));
    }

    std::vector<VkCommandBuffer> commandBuffers;
    for (CommandBufferID commandBuffer : inCommandBuffers)
    {
        commandBuffers.emplace_back(reinterpret_cast<VkCommandBuffer>(commandBuffer.id));
    }

    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    submitInfo.waitSemaphoreCount = waitSemaphores.size();
    submitInfo.pWaitSemaphores = waitSemaphores.data();;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = commandBuffers.size();
    submitInfo.pCommandBuffers = commandBuffers.data();
    submitInfo.signalSemaphoreCount = signalSemaphores.size();
    submitInfo.pSignalSemaphores = signalSemaphores.data();

    VK_CHECK(vkQueueSubmit(VulkanContext::getDevice()->getGraphicsQueue(), 1, &submitInfo, reinterpret_cast<VkFence>(fence.id)));
}

void VulkanContext::queuePresent(const std::vector<SemaphoreID>& inWaitSemaphore, const uint32_t imageIndex)
{
    std::vector<VkSemaphore> waitSemaphores;
    for (SemaphoreID semaphore : inWaitSemaphore)
    {
        waitSemaphores.emplace_back(reinterpret_cast<VkSemaphore>(semaphore.id));
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = waitSemaphores.size();
    presentInfo.pWaitSemaphores = waitSemaphores.data();
    VkSwapchainKHR swapChains[] = { VulkanContext::getSwapChain()->getVkSwapChain() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr; // Optional

    VK_CHECK(vkQueuePresentKHR(VulkanContext::getDevice()->getPresentQueue(), &presentInfo));
}

const std::vector<const char*>& VulkanContext::getInstanceExtensions() 
{
    return m_instanceExtensions;
}

const std::vector<const char*>& VulkanContext::getDeviceExtensions()
{
    return m_deviceExtensions;
}

const std::vector<const char*>& VulkanContext::getInstanceLayers()
{
    return m_instanceLayers;
}
