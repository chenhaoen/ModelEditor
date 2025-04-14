#include "Core/SkyboxNode.h"
#include "Core/Mesh.h"
#include "Core/Material.h"
#include "Core/PipelineManager.h"
#include "Core/FrameManager.h"
#include "Core/Commands/CommandGroup.h"

SkyboxNode::SkyboxNode()
	:Node("skybox")
{
}

SkyboxNode::~SkyboxNode()
{
}

void SkyboxNode::record()
{
    PipelineManager::instance()->setCurrentPipeline(PipelineType::Skybox);
    FrameManager::instance()->currentCommandGroup()->push(PipelineManager::instance()->getCommands());
    m_material->record();
    PipelineManager::instance()->updateDescriptorSets();
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
