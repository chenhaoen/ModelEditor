#include "Core/Node.h"
#include "Core/Mesh.h"
#include "Core/Material.h"
#include "Core/PipelineManager.h"
#include "Core/FrameManager.h"
#include "Core/SceneManager.h"
#include "Core/Pipeline.h"

#include "Core/Commands/BindPipelineCommand.h"
#include "Core/Commands/CommandGroup.h"
#include <Core/Commands/BindDescriptorSetsCommand.h>

#include "Core/RenderingContextDriver/RenderingContextDriver.h"

struct Lights
{
	std::array<glm::vec4, 4> light;
};

struct PushConsts
{
	float roughness;
	float metallic;
	float r;
	float g;
	float b;
};

Node::Node(const std::string& name)
	:m_parent(nullptr)
	, m_name(name)
{

}

Node::~Node()
{
	RenderingContextDriver::instance()->freeBuffer(m_lightBuffer);
	RenderingContextDriver::instance()->freeBuffer(m_materialBuffer);
}

void Node::setParent(Node* parent)
{
	m_parent = parent;
}

void Node::addChild(std::shared_ptr<Node> child)
{
	auto where = std::find_if(m_children.begin(), m_children.end(), [&](const std::shared_ptr<Node>& node) {
		return node == child;
		});

	if (where != m_children.end())
	{
		return;
	}

	m_children.emplace_back(child);
	child->setParent(this);
}

void Node::removeChild(std::shared_ptr<Node> child)
{
	auto where = std::find_if(m_children.begin(), m_children.end(), [&](const std::shared_ptr<Node>& node) {
		return node == child;
		});

	if (where == m_children.end())
	{
		return;
	}

	child->setParent(nullptr);
	m_children.erase(where);
}

const std::string& Node::getName() const
{
	return m_name;
}

void Node::setMesh(std::shared_ptr<Mesh> mesh)
{
	m_mesh = mesh;
}

std::shared_ptr<Mesh> Node::getMesh() const
{
	return m_mesh;
}

void Node::setMaterial(std::shared_ptr<Material> material)
{
	m_material = material;
}

std::shared_ptr<Material> Node::getMaterial() const
{
	return m_material;
}

void Node::record()
{
	if (m_mesh)
	{
		PipelineManager::instance()->getPipeline(PipelineName::PBR)->bind();
		FrameManager::instance()->currentCommandGroup()->push(std::make_shared<BindPipelineCommand>(PipelineName::PBR));
		FrameManager::instance()->currentCommandGroup()->push(PipelineManager::instance()->getCommands(PipelineName::PBR));
		FrameManager::instance()->currentCommandGroup()->push(std::make_shared<BindDescriptorSetsCommand>(PipelineName::PBR));
		
	
		
		PipelineManager::instance()->updateDescriptorSets(PipelineName::PBR);

		//PipelineManager::instance()->getPipeline(PipelineName::Model)->bind();
		//FrameManager::instance()->currentCommandGroup()->push(std::make_shared<BindPipelineCommand>(PipelineName::Model));
		//FrameManager::instance()->currentCommandGroup()->push(PipelineManager::instance()->getCommands(PipelineName::Model));
		//FrameManager::instance()->currentCommandGroup()->push(std::make_shared<BindDescriptorSetsCommand>(PipelineName::Model));
		//
		//{
		//	Lights lights;
		//	const float p = 15.0f;
		//	lights.light[0] = glm::vec4(-p, -p * 0.5f, -p, 1.0f);
		//	lights.light[1] = glm::vec4(-p, -p * 0.5f, p, 1.0f);
		//	lights.light[2] = glm::vec4(p, -p * 0.5f, p, 1.0f);
		//	lights.light[3] = glm::vec4(p, -p * 0.5f, -p, 1.0f);
		//
		//	RenderingContextDriver::instance()->updateUniformBuffer(m_lightBuffer, &lights, sizeof(Lights));
		//}
		//BoundUniform boundUniform;
		//boundUniform.binding = 1;
		//boundUniform.type = UniformType::UNIFORM_TYPE_UNIFORM_BUFFER;
		//boundUniform.ids.push_back(m_lightBuffer);
		//boundUniform.ids.push_back(sizeof(UniformBufferObject));
		//boundUniform.ids.push_back(sizeof(Lights));
		//PipelineManager::instance()->getPipeline(PipelineName::Model)->m_boundUniforms.push_back(boundUniform);
		//
		//{
		//	PushConsts pushconsts;
		//	pushconsts.metallic = 0.1;
		//	pushconsts.roughness = 1.0;
		//	pushconsts.r = 1.0f;
		//	pushconsts.g = 0.765557f;
		//	pushconsts.b = 0.336057f;
		//	RenderingContextDriver::instance()->updateUniformBuffer(m_materialBuffer, &pushconsts, sizeof(PushConsts));
		//}
		//boundUniform.binding = 2;
		//boundUniform.type = UniformType::UNIFORM_TYPE_UNIFORM_BUFFER;
		//boundUniform.ids.clear();
		//boundUniform.ids.push_back(m_materialBuffer);
		//boundUniform.ids.push_back(sizeof(UniformBufferObject) + sizeof(Lights));
		//boundUniform.ids.push_back(sizeof(PushConsts));
		//PipelineManager::instance()->getPipeline(PipelineName::Model)->m_boundUniforms.push_back(boundUniform);
		//
		//PipelineManager::instance()->updateDescriptorSets(PipelineName::Model);
		m_mesh->record();
	}

	for (auto child : m_children)
	{
		child->record();
	}
}

void Node::compile()
{
	if (m_mesh)
	{
		m_mesh->compile();
	}

	if (!m_lightBuffer)
	{
		m_lightBuffer = RenderingContextDriver::instance()->createUniformBuffer(sizeof(Lights));
	}

	if (!m_materialBuffer)
	{
		m_materialBuffer = RenderingContextDriver::instance()->createUniformBuffer(sizeof(PushConsts));
	}

	for (auto child : m_children)
	{
		child->compile();
	}
}
