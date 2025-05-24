#include "Core/SkyboxNode.h"
#include "Core/Mesh.h"
#include "Core/Material.h"
#include "Core/PipelineManager.h"
#include "Core/FrameManager.h"
#include "Core/Pipeline.h"

#include "Core/Commands/CommandGroup.h"
#include "Core/Commands/BindPipelineCommand.h"
#include "Core/Commands/BindDescriptorSetsCommand.h"

SkyboxNode::SkyboxNode()
	:Node("skybox")
{
}

SkyboxNode::~SkyboxNode()
{
}

void SkyboxNode::record()
{
    PipelineManager::instance()->getPipeline(PipelineName::Skybox)->bind();
    FrameManager::instance()->currentCommandGroup()->push(std::make_shared<BindPipelineCommand>(PipelineName::Skybox));
    FrameManager::instance()->currentCommandGroup()->push(PipelineManager::instance()->getCommands(PipelineName::Skybox));
    FrameManager::instance()->currentCommandGroup()->push(std::make_shared<BindDescriptorSetsCommand>(PipelineName::Skybox));
    PipelineManager::instance()->getPipeline(PipelineName::Skybox)->m_boundUniforms.push_back(m_material->getBoundUniform());
    PipelineManager::instance()->updateDescriptorSets(PipelineName::Skybox);
    m_mesh->record();
}

void SkyboxNode::compile()
{
    if (m_material && m_mesh)
    {
        m_material->compile();
        m_mesh->compile();
    }
}
