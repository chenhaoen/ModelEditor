#pragma once

#include <iostream>
#include <string_view>
#include <array>

#include <vulkan/vk_enum_string_helper.h>

#define VK_CHECK(result)                                                 \
    {                                                                    \
        VkResult res = (result);                                        \
        if (res != VK_SUCCESS) {                                        \
            std::cerr << "Vulkan error: " << string_VkResult(res)       \
                        << " at " << __FILE__  \
                      << ":" << __LINE__ << std::endl;                  \
        }                                                                \
    }

constexpr std::string_view g_modelPipelineName("Model");

VkVertexInputBindingDescription getBindingDescription();

std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();