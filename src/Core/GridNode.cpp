#include "Core/GridNode.h"
#include "Core/Mesh.h"

GridNode::GridNode()
	:Node("Grids")
{
}

void GridNode::record()
{
   // PipelineManager::instance()->getPipeline(PipelineType::Skybox)->bind();
   // FrameManager::instance()->currentCommandGroup()->push(std::make_shared<BindPipelineCommand>(PipelineType::Skybox));
   // FrameManager::instance()->currentCommandGroup()->push(PipelineManager::instance()->getCommands(PipelineType::Skybox));
   // FrameManager::instance()->currentCommandGroup()->push(std::make_shared<BindDescriptorSetsCommand>(PipelineType::Skybox));
   // PipelineManager::instance()->getPipeline(PipelineType::Skybox)->m_boundUniforms.push_back(m_material->getBoundUniform());
   // PipelineManager::instance()->updateDescriptorSets(PipelineType::Skybox);
    if (m_mesh)
    {
        m_mesh->record();
    }
}

void GridNode::compile()
{
}
