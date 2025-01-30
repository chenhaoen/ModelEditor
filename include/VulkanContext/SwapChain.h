#pragma once

#include <vector>

#include <vulkan/vulkan.h>



class SwapChain
{
public:
    SwapChain();

    ~SwapChain();

    void create();
    void createImageViews();
    void createFrameBuffers();

    void createDepthResources();

    void cleanup();

    VkSwapchainKHR getVkSwapChain() const;

    VkFramebuffer getFrameBuffer(const int index);

    uint32_t getNextImageIndex(VkSemaphore semaphore);
private:
    VkSwapchainKHR m_vkSwapChain;

    std::vector<VkImage> m_images;
    std::vector<VkImageView> m_imageViews;

    std::vector<VkFramebuffer> m_frameBuffers;

    VkImage m_depthImage;
    VkDeviceMemory m_depthImageMemory;
    VkImageView m_depthImageView;
};
