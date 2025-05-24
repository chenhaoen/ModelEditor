#include "VulkanContext/utils.h"
#include "VulkanContext/VulkanContext.h"
#include "VulkanContext/LogicalDevice.h"
#include "VulkanContext/PhysicalDevice.h"
#include "VulkanContext/CommandPool.h"

#include "Core/RenderingContextDriver/Commons.h"

VkVertexInputBindingDescription getBindingDescription()
{
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, pos);
    
    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

    return attributeDescriptions;
}

void createVKBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VK_CHECK(vkCreateBuffer(VulkanContext::getDevice()->getVkDevice(), &bufferInfo, nullptr, &buffer));

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(VulkanContext::getDevice()->getVkDevice(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = VulkanContext::getPhysicalDevice()->findMemoryType(memRequirements.memoryTypeBits, properties);

    VK_CHECK(vkAllocateMemory(VulkanContext::getDevice()->getVkDevice(), &allocInfo, nullptr, &bufferMemory));

    vkBindBufferMemory(VulkanContext::getDevice()->getVkDevice(), buffer, bufferMemory, 0);
}

void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBuffer commandBuffer = VulkanContext::getCommandPool()->beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    VulkanContext::getCommandPool()->endSingleTimeCommands(commandBuffer);
}

std::vector<char> readFile(const std::string_view& filename)
{
    std::ifstream file(filename.data(), std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

VkShaderModule createShaderModule(const std::vector<char>& code)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    VK_CHECK(vkCreateShaderModule(VulkanContext::getDevice()->getVkDevice(), &createInfo, nullptr, &shaderModule));
    return shaderModule;
}

VkShaderStageFlagBits ShaderTypeToVk(ShaderType shaderType)
{
    switch (shaderType)
    {
    case ShaderType::Vertex:
        return VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
        break;
    case ShaderType::Fragment:
        return VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
        break;
    case ShaderType::Compute:
        return VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT;
        break;
    default:
        break;
    }

    return VkShaderStageFlagBits::VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
}

VkDescriptorType UniformTypeToVK(UniformType uniformType)
{
    switch (uniformType)
    {
    case UniformType::UNIFORM_TYPE_SAMPLER:
        break;
    case UniformType::UNIFORM_TYPE_SAMPLER_WITH_TEXTURE:
        return VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        break;
    case UniformType::UNIFORM_TYPE_TEXTURE:
        break;
    case UniformType::UNIFORM_TYPE_IMAGE:
        break;
    case UniformType::UNIFORM_TYPE_TEXTURE_BUFFER:
        break;
    case UniformType::UNIFORM_TYPE_SAMPLER_WITH_TEXTURE_BUFFER:
        break;
    case UniformType::UNIFORM_TYPE_IMAGE_BUFFER:
        break;
    case UniformType::UNIFORM_TYPE_UNIFORM_BUFFER:
        return VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        break;
    case UniformType::UNIFORM_TYPE_STORAGE_BUFFER:
        return VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        break;
    case UniformType::UNIFORM_TYPE_INPUT_ATTACHMENT:
        break;
    case UniformType::UNIFORM_TYPE_MAX:
        break;
    default:
        break;
    }

    return VkDescriptorType::VK_DESCRIPTOR_TYPE_MAX_ENUM;
}
