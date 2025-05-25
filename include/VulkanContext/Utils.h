#pragma once

#include "Core/Shader/Shader.h"

#define VK_CHECK(result)                                                 \
    {                                                                    \
        VkResult res = (result);                                        \
        if (res != VK_SUCCESS) {                                        \
            std::cerr << "Vulkan error: " << string_VkResult(res)       \
                        << " at " << __FILE__  \
                      << ":" << __LINE__ << std::endl;                  \
        }                                                                \
    }

void createVKBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

std::vector<char> readFile(const std::string_view& filename);

VkShaderModule createShaderModule(const std::vector<char>& code);

VkShaderStageFlagBits ShaderTypeToVk(ShaderType shaderType);

VkDescriptorType UniformTypeToVK(UniformType uniformType);

void RasterizationStateToVK(VkPipelineRasterizationStateCreateInfo& , const PipelineRasterizationState& );

void InputAssemblyStateToVK(VkPipelineInputAssemblyStateCreateInfo& , const RenderPrimitive);

void MultisampleStateToVK(VkPipelineMultisampleStateCreateInfo&, const PipelineMultisampleState&);

void VertexInputStateToVK(std::vector<VkVertexInputBindingDescription>& bindings,
std::vector<VkVertexInputAttributeDescription>& attributes, const VertexInputState&);