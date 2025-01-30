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
#include "VulkanContext/DescriptorSetLayout.h"
#include "VulkanContext/Pipeline.h"

VulkanContext *VulkanContext::g_instance = nullptr;

VulkanContext::VulkanContext()
    :m_instance(nullptr)
    , m_debugUtilsMessenger(nullptr)
    , m_physicalDevice(nullptr)
    , m_surface(nullptr)
    , m_device(nullptr)
    , m_renderPass(nullptr)
    , m_swapChain(nullptr)
    , m_descriptorSetLayout(nullptr)
    , m_pipeLine(nullptr)
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

VkCommandPool commandPool;
std::vector<VkCommandBuffer> commandBuffers;

std::vector<VkSemaphore> imageAvailableSemaphores;
std::vector<VkSemaphore> renderFinishedSemaphores;
std::vector<VkFence> inFlightFences;
uint32_t currentFrame = 0;

void createCommandPool() {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = VulkanContext::instance()->getPhysicalDevice()->getGraphicsFamilyIndex();;

    if (vkCreateCommandPool(VulkanContext::instance()->getDevice()->getVkDevice() , &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

void createCommandBuffers() {
    commandBuffers.resize(VulkanContext::getMaxFrameCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

    if (vkAllocateCommandBuffers(VulkanContext::instance()->getDevice()->getVkDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void createSyncObjects() {
    imageAvailableSemaphores.resize(VulkanContext::getMaxFrameCount());
    renderFinishedSemaphores.resize(VulkanContext::getMaxFrameCount());
    inFlightFences.resize(VulkanContext::getMaxFrameCount());

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < VulkanContext::getMaxFrameCount(); i++) {
        if (vkCreateSemaphore(VulkanContext::instance()->getDevice()->getVkDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(VulkanContext::instance()->getDevice()->getVkDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(VulkanContext::instance()->getDevice()->getVkDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }
}

void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = VulkanContext::instance()->getRenderPass()->getVkRenderPass();
    renderPassInfo.framebuffer = VulkanContext::instance()->getSwapChain()->getFrameBuffer(imageIndex);
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = VulkanContext::instance()->getSurface()->getExtent();

    VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanContext::instance()->getPipeline()->getVkPipeline());

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)VulkanContext::instance()->getSurface()->getExtent().width;
    viewport.height = (float)VulkanContext::instance()->getSurface()->getExtent().height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = VulkanContext::instance()->getSurface()->getExtent();
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    vkCmdDraw(commandBuffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
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

    m_descriptorSetLayout = new DescriptorSetLayout();

    m_pipeLine = new Pipeline(m_descriptorSetLayout);

    createCommandPool();
    createCommandBuffers();
    createSyncObjects();
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
    for (size_t i = 0; i < getMaxFrameCount(); i++) {
        vkDestroySemaphore(m_device->getVkDevice(), renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(m_device->getVkDevice(), imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(m_device->getVkDevice(), inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(m_device->getVkDevice(), commandPool, nullptr);

    delete m_pipeLine;
    delete m_descriptorSetLayout;
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
    vkWaitForFences(m_device->getVkDevice() , 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
    vkResetFences(m_device->getVkDevice(), 1, &inFlightFences[currentFrame]);

    uint32_t imageIndex = m_swapChain->getNextImageIndex(imageAvailableSemaphores[currentFrame]);;

    vkResetCommandBuffer(commandBuffers[currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
    recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

    VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(m_device->getGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { m_swapChain->getVkSwapChain()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(m_device->getPresentQueue(), &presentInfo);

    currentFrame = (currentFrame + 1) % getMaxFrameCount();
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
