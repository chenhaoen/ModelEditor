#include "Core/BackgroundNode.h"
#include "Core/Mesh.h"
#include "Core/PipelineManager.h"
#include "Core/FrameManager.h"
#include "Core/Commands/BindPipelineCommand.h"
#include "Core/Commands/CommandGroup.h"

#include "Core/RenderingContextDriver/Commons.h"

BackgroundNode::BackgroundNode()
	:Node("Background")
{
}

BackgroundNode::~BackgroundNode()
{
}

void BackgroundNode::record()
{
    FrameManager::instance()->currentCommandGroup()->push(std::make_shared<BindPipelineCommand>(PipelineName::Background));

    FrameManager::instance()->currentCommandGroup()->push(PipelineManager::instance()->getCommands(PipelineName::Background));
    PipelineManager::instance()->updateDescriptorSets(PipelineName::Background);

    m_mesh->record();
}

void BackgroundNode::compile()
{
	if (m_compiled)
	{
		return;
	}

    m_mesh = std::make_shared<Mesh>();

    m_compiled = true;

    std::vector<Vertex> vertices;
    Vertex vertex;
    vertex.pos = { -1.0f, -1.0f, 0.0f };
    vertex.texCoord = { 0.0f, 0.0f, 0.0f };
    vertices.emplace_back(vertex);

    vertex.pos = { 1.0f, -1.0f, 0.0f };
    vertex.texCoord = { 1.0f, 0.0f, 0.0f };
    vertices.emplace_back(vertex);

    vertex.pos = { -1.0f,  1.0f , 0.0f };
    vertex.texCoord = { 0.0f, 1.0f, 0.0f };
    vertices.emplace_back(vertex);

    vertex.pos = { 1.0f,  1.0f , 0.0f };
    vertex.texCoord = { 1.0f, 1.0f, 0.0f };
    vertices.emplace_back(vertex);
  
    m_mesh->setVertices(vertices);

    const std::vector<uint32_t> indices = {
    0, 1, 2,  // 第一个三角形（左下、右下、左上）
    2, 1, 3   // 第二个三角形（左上、右下、右上）
    };

    m_mesh->setIndices(indices);

    m_mesh->compile();


}
