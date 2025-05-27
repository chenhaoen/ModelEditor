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
    //RenderingContextDriver::instance()->freeBuffer(m_indexBuffer);
    //RenderingContextDriver::instance()->freeBuffer(m_vertexBuffer);
}

void GridNode::record()
{
    PipelineManager::instance()->getPipeline(PipelineName::GridsGraphics)->bind();
    
    
    FrameManager::instance()->currentCommandGroup()->push(std::make_shared<BindPipelineCommand>(PipelineName::GridsGraphics));
    
    FrameManager::instance()->currentCommandGroup()->push(PipelineManager::instance()->getCommands(PipelineName::GridsGraphics));
    PipelineManager::instance()->updateDescriptorSets(PipelineName::GridsGraphics);
    
    FrameManager::instance()->currentCommandGroup()->push(std::make_shared<BindDescriptorSetsCommand>(PipelineName::GridsGraphics));
    m_mesh->record();
}

void GridNode::compile()
{
    if (m_compiled)
    {
        return;
    }

    m_compiled = true;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    GenerateGridMesh(10, 10.0f, vertices, indices, false);

    m_mesh = std::make_shared<Mesh>();
    m_mesh->setVertices(vertices);
    m_mesh->setIndices(indices);
    m_mesh->m_primitiveType = PrimitiveType::Lines;
    m_mesh->m_drawMode = DrawMode::Indexed;
    m_mesh->compile();

    //FrameManager::instance()->currentCommandGroup()->push(std::make_shared<BindPipelineCommand>(PipelineName::Grids));
    //FrameManager::instance()->currentCommandGroup()->push(PipelineManager::instance()->getCommands(PipelineName::Grids));
    //FrameManager::instance()->currentCommandGroup()->push(std::make_shared<BindDescriptorSetsCommand>(PipelineName::Grids));
    //PipelineManager::instance()->updateDescriptorSets(PipelineName::Grids);

    //uint64_t vertexBufferSize = sizeof(float) * 3 * gridSize * gridSize;
    //uint64_t indexBufferSize = sizeof(uint32_t) * (gridSize - 1) * (gridSize - 1) * 6;
    //
    //m_vertexBuffer = RenderingContextDriver::instance()->createStorageBuffer(vertexBufferSize,
    //    BufferUsageBits::BUFFER_USAGE_STORAGE_BIT | BufferUsageBits::BUFFER_USAGE_VERTEX_BIT);
    //
    //m_indexBuffer = RenderingContextDriver::instance()->createStorageBuffer(indexBufferSize,
    //    BufferUsageBits::BUFFER_USAGE_STORAGE_BIT | BufferUsageBits::BUFFER_USAGE_INDEX_BIT);
    //
    //BoundUniform vertexUniform;
    //vertexUniform.binding = 0;
    //vertexUniform.type = UniformType::UNIFORM_TYPE_STORAGE_BUFFER;
    //vertexUniform.ids.push_back(m_vertexBuffer);
    //PipelineManager::instance()->getPipeline(PipelineName::Grids)->m_boundUniforms.push_back(vertexUniform);
    //
    //BoundUniform indexUniform;
    //indexUniform.binding = 1;
    //indexUniform.type = UniformType::UNIFORM_TYPE_STORAGE_BUFFER;
    //indexUniform.ids.push_back(m_indexBuffer);
    //PipelineManager::instance()->getPipeline(PipelineName::Grids)->m_boundUniforms.push_back(indexUniform);
    //
    //PipelineManager::instance()->updateDescriptorSets(PipelineName::Grids);
    //
    //CommandBufferID  commandBuffer = RenderingContextDriver::instance()->createCommandBuffer();
    //
    //RenderingContextDriver::instance()->beginCommandBuffer(commandBuffer);
    //
    //FrameManager::instance()->currentCommandGroup()->push(std::make_shared<BindPipelineCommand>(PipelineName::Grids));
    //FrameManager::instance()->currentCommandGroup()->push(PipelineManager::instance()->getCommands(PipelineName::Grids));
    //FrameManager::instance()->currentCommandGroup()->push(std::make_shared<BindDescriptorSetsCommand>(PipelineName::Grids));
    //
    //FrameManager::instance()->currentCommandGroup()->record(commandBuffer);
    //
    //struct {
    //    uint32_t gridSize;
    //    float cellSize;
    //} pushConstants;
    //
    //pushConstants.gridSize = gridSize;
    //pushConstants.cellSize = cellSize;
    //RenderingContextDriver::instance()->cmdPushConstants(commandBuffer, PipelineManager::instance()->getPipeline(PipelineName::Grids)->m_id,
    //    sizeof(pushConstants), &pushConstants, ShaderType::Compute);
    //
    //// 分发计算着色器
    //uint32_t groupCountX = (gridSize + 7) / 8;
    //uint32_t groupCountY = (gridSize + 7) / 8;
    //RenderingContextDriver::instance()->cmdDispatch(commandBuffer, groupCountX, groupCountY, 1);
    //
    //RenderingContextDriver::instance()->endCommandBuffer(commandBuffer);
    //
    //FenceID fence = RenderingContextDriver::instance()->createFence(false);
    //RenderingContextDriver::instance()->queueSubmit({ commandBuffer }, {  }, {  }, fence);
    //RenderingContextDriver::instance()->waitFence(fence);
    //RenderingContextDriver::instance()->freeFence(fence);

}

void GridNode::GenerateGridMesh(int gridSize, float cellSize, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, bool includeDiagonals)
{
    vertices.clear();
    indices.clear();

    const float halfSize = gridSize * cellSize * 0.5f;
    const glm::vec3 gridColor(0.5f, 0.5f, 0.5f); // 网格线颜色

    // 生成水平线和垂直线
    for (int i = 0; i <= gridSize; ++i) {
        // 垂直线
        float x = i * cellSize - halfSize;
        vertices.push_back({ {x, 0.0f, -halfSize}, gridColor });
        vertices.push_back({ {x, 0.0f,  halfSize}, gridColor });

        // 水平线
        float z = i * cellSize - halfSize;
        vertices.push_back({ {-halfSize, 0.0f, z}, gridColor });
        vertices.push_back({ { halfSize, 0.0f, z}, gridColor });

        // 添加线段的索引 (线列表模式)
        uint32_t baseIdx = i * 4;
        indices.push_back(baseIdx);
        indices.push_back(baseIdx + 1);
        indices.push_back(baseIdx + 2);
        indices.push_back(baseIdx + 3);
    }

    // 如果需要，添加对角线
    if (includeDiagonals) {
        const glm::vec3 diagonalColor(0.8f, 0.2f, 0.2f);

        // 主对角线
        vertices.push_back({ {-halfSize, 0.0f, -halfSize}, diagonalColor });
        vertices.push_back({ { halfSize, 0.0f,  halfSize}, diagonalColor });

        // 副对角线
        vertices.push_back({ {-halfSize, 0.0f,  halfSize}, diagonalColor });
        vertices.push_back({ { halfSize, 0.0f, -halfSize}, diagonalColor });

        // 添加对角线索引
        uint32_t baseIdx = vertices.size() - 4;
        indices.push_back(baseIdx);
        indices.push_back(baseIdx + 1);
        indices.push_back(baseIdx + 2);
        indices.push_back(baseIdx + 3);
    }
}
