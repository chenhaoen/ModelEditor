#include "Core/GridNode.h"
#include "Core/Mesh.h"

#include "Core/PipelineManager.h"
#include "Core/Pipeline.h"
#include "Core/Commands/CommandGroup.h"
#include "Core/Commands/BindDescriptorSetsCommand.h"
#include "Core/Commands/BindPipelineCommand.h"
#include "Core/Commands/BindBufferCommand.h"
#include "Core/Commands/DrawCommand.h"
#include "Core/RenderingContextDriver/RenderingContextDriver.h"

namespace {
    // 网格参数
    uint32_t gridSize = 32;
    float cellSize = 1.0f;
}

GridNode::GridNode()
	:Node("Grids")
{
}

GridNode::~GridNode()
{
    RenderingContextDriver::instance()->freeBuffer(m_indexBuffer);
    RenderingContextDriver::instance()->freeBuffer(m_vertexBuffer);
}

void GridNode::record()
{
    auto commandGroup = std::make_shared<CommandGroup>();
    commandGroup->push(std::make_shared<BindBufferCommand>(m_vertexBuffer, BufferType::Vertex));
    commandGroup->push(std::make_shared<BindBufferCommand>(m_indexBuffer, BufferType::Index));

    // 绘制网格
    uint32_t indexCount = (gridSize - 1) * (gridSize - 1) * 6;

    commandGroup->push(std::make_shared<DrawCommand>(DrawMode::Indexed, indexCount, 1, 0, 0, 0));

    FrameManager::instance()->currentCommandGroup()->push(commandGroup);
}

void GridNode::compile()
{
    if (m_compiled)
    {
        return;
    }

    m_compiled = true;

    //FrameManager::instance()->currentCommandGroup()->push(std::make_shared<BindPipelineCommand>(PipelineName::Grids));
    //FrameManager::instance()->currentCommandGroup()->push(PipelineManager::instance()->getCommands(PipelineName::Grids));
    //FrameManager::instance()->currentCommandGroup()->push(std::make_shared<BindDescriptorSetsCommand>(PipelineName::Grids));
    //PipelineManager::instance()->updateDescriptorSets(PipelineName::Grids);

    uint64_t vertexBufferSize = sizeof(float) * 3 * gridSize * gridSize;
    uint64_t indexBufferSize = sizeof(uint32_t) * (gridSize - 1) * (gridSize - 1) * 6;

    m_vertexBuffer = RenderingContextDriver::instance()->createStorageBuffer(vertexBufferSize,
        BufferUsageBits::BUFFER_USAGE_STORAGE_BIT | BufferUsageBits::BUFFER_USAGE_VERTEX_BIT);

    m_indexBuffer = RenderingContextDriver::instance()->createStorageBuffer(indexBufferSize,
        BufferUsageBits::BUFFER_USAGE_STORAGE_BIT | BufferUsageBits::BUFFER_USAGE_INDEX_BIT);

    BoundUniform vertexUniform;
    vertexUniform.binding = 0;
    vertexUniform.type = UniformType::UNIFORM_TYPE_STORAGE_BUFFER;
    vertexUniform.ids.push_back(m_vertexBuffer);
    PipelineManager::instance()->getPipeline(PipelineName::Grids)->m_boundUniforms.push_back(vertexUniform);

    BoundUniform indexUniform;
    indexUniform.binding = 1;
    indexUniform.type = UniformType::UNIFORM_TYPE_STORAGE_BUFFER;
    indexUniform.ids.push_back(m_indexBuffer);
    PipelineManager::instance()->getPipeline(PipelineName::Grids)->m_boundUniforms.push_back(indexUniform);

    PipelineManager::instance()->updateDescriptorSets(PipelineName::Grids);

    CommandBufferID  commandBuffer = RenderingContextDriver::instance()->createCommandBuffer();

    RenderingContextDriver::instance()->beginCommandBuffer(commandBuffer);

    FrameManager::instance()->currentCommandGroup()->push(std::make_shared<BindPipelineCommand>(PipelineName::Grids));
    FrameManager::instance()->currentCommandGroup()->push(PipelineManager::instance()->getCommands(PipelineName::Grids));
    FrameManager::instance()->currentCommandGroup()->push(std::make_shared<BindDescriptorSetsCommand>(PipelineName::Grids));

    FrameManager::instance()->currentCommandGroup()->record(commandBuffer);

    struct {
        uint32_t gridSize;
        float cellSize;
    } pushConstants;

    pushConstants.gridSize = gridSize;
    pushConstants.cellSize = cellSize;
    RenderingContextDriver::instance()->cmdPushConstants(commandBuffer, PipelineManager::instance()->getPipeline(PipelineName::Grids)->m_id,
        sizeof(pushConstants), &pushConstants, ShaderType::Compute);

    // 分发计算着色器
    uint32_t groupCountX = (gridSize + 7) / 8;
    uint32_t groupCountY = (gridSize + 7) / 8;
    RenderingContextDriver::instance()->cmdDispatch(commandBuffer, groupCountX, groupCountY, 1);

    RenderingContextDriver::instance()->endCommandBuffer(commandBuffer);

    FenceID fence = RenderingContextDriver::instance()->createFence(false);
    RenderingContextDriver::instance()->queueSubmit({ commandBuffer }, {  }, {  }, fence);
    RenderingContextDriver::instance()->waitFence(fence);
    RenderingContextDriver::instance()->freeFence(fence);

}
