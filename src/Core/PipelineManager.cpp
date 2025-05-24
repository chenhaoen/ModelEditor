#include "Core/PipelineManager.h"
#include "Core/Pipeline.h"
#include "Core/Shader/ShaderManager.h"

#include "Core/RenderingContextDriver/RenderingContextDriver.h"

PipelineManager* PipelineManager::g_instance = nullptr;

PipelineManager* PipelineManager::instance()
{
	if (!g_instance)
	{
		g_instance = new PipelineManager();
	}

	return g_instance;
}

void PipelineManager::init()
{
	ShaderManager::instance()->init();

	//Model
	{
		PipelineCreateInfo pipelineCreateInfo;

		pipelineCreateInfo.m_dynamicStates.push_back(DynamicStateType::Scissor);
		pipelineCreateInfo.m_dynamicStates.push_back(DynamicStateType::Viewport);
		pipelineCreateInfo.m_dynamicStates.push_back(DynamicStateType::PolygonMode);
		pipelineCreateInfo.m_shaders.push_back(ShaderManager::instance()->getShader("Model.vert"));
		pipelineCreateInfo.m_shaders.push_back(ShaderManager::instance()->getShader("Model.frag"));

		pipelineCreateInfo.m_renderPrimitive = RenderPrimitive::RENDER_PRIMITIVE_TRIANGLES;
		auto pipeline = std::make_shared<Pipeline>(pipelineCreateInfo);
		addPipeline(PipelineName::Model, pipeline);
	}

	// skybox
	{
		PipelineCreateInfo pipelineCreateInfo;
		
		pipelineCreateInfo.m_shaders.push_back(ShaderManager::instance()->getShader("skybox.vert"));
		pipelineCreateInfo.m_shaders.push_back(ShaderManager::instance()->getShader("skybox.frag"));

		pipelineCreateInfo.m_dynamicStates.push_back(DynamicStateType::Scissor);
		pipelineCreateInfo.m_dynamicStates.push_back(DynamicStateType::Viewport);
		pipelineCreateInfo.m_renderPrimitive = RenderPrimitive::RENDER_PRIMITIVE_TRIANGLES;
		auto pipeline = std::make_shared<Pipeline>(pipelineCreateInfo);
		addPipeline(PipelineName::Skybox, pipeline);
	}

	// Grids
	{
		PipelineCreateInfo pipelineCreateInfo;

		pipelineCreateInfo.m_shaders.push_back(ShaderManager::instance()->getShader("grids.compute"));
		pipelineCreateInfo.m_type = PipelineType::Compute;
		pipelineCreateInfo.m_dynamicStates.push_back(DynamicStateType::Scissor);
		pipelineCreateInfo.m_dynamicStates.push_back(DynamicStateType::Viewport);
		pipelineCreateInfo.m_renderPrimitive = RenderPrimitive::RENDER_PRIMITIVE_TRIANGLES;
		auto pipeline = std::make_shared<Pipeline>(pipelineCreateInfo);
		addPipeline(PipelineName::Grids, pipeline);
	}
}

std::shared_ptr<Pipeline> PipelineManager::getPipeline(const PipelineName name)
{
	return m_pipelines[name];
}

void PipelineManager::destroy()
{
	delete g_instance;
	g_instance = nullptr;
}

uint32_t PipelineManager::pipelineCount() const
{
	return m_pipelines.size();
}

std::shared_ptr<CommandGroup> PipelineManager::getCommands(const PipelineName name)
{
	return m_pipelines[name]->getCommands();
}

void PipelineManager::updateDescriptorSets(const PipelineName name)
{
	m_pipelines[name]->updateDescriptorSets();
}

PipelineManager::PipelineManager()
{

}

PipelineManager::~PipelineManager()
{
}

void PipelineManager::addPipeline(const PipelineName name, std::shared_ptr<Pipeline> pipeline)
{
	m_pipelines.emplace(name, pipeline);
}
