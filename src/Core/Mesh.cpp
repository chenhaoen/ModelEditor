#include "Core/Mesh.h"
#include "Core/Image.h"
#include "Core/FrameManager.h"
#include "Core/SceneManager.h"
#include "Core/Scene.h"

#include "Core/Commands/BindBufferCommand.h"
#include "Core/Commands/CommandGroup.h"
#include "Core/Commands/DrawCommand.h"

#include "Core/RenderingContextDriver/RenderingContextDriver.h"

Mesh::Mesh()
	: m_drawMode(DrawMode::Indexed)
	, m_isChanged(true)
	, m_primitiveType(PrimitiveType::Triangles)
{
}

Mesh::~Mesh()
{
	freeVertexBuffer();
	freeIndexBuffer();
}

void Mesh::setVertices(const std::vector<Vertex>& vertices)
{
	m_vertices = vertices;
}

std::vector<Vertex>& Mesh::getVertices()
{
	return m_vertices;
}

const std::vector<Vertex>& Mesh::getVertices() const
{
	return m_vertices;
}

void Mesh::setIndices(const std::vector<uint32_t>& indices)
{
	m_indices = indices;
}

std::vector<uint32_t>& Mesh::getIndices()
{
	return m_indices;
}

const std::vector<uint32_t>& Mesh::getIndices() const
{
	return m_indices;
}

bool Mesh::isChanged()
{
	return m_isChanged;
}

void Mesh::setChanged(const bool value)
{
	m_isChanged = value;
}

void Mesh::compile()
{
	if (m_compiled)
	{
		return;
	}

	createVertexBuffer();
	createIndexBuffer();

	m_compiled = true;
}

void Mesh::createVertexBuffer()
{
	if (m_vertices.empty())
	{
		return;
	}

	m_vertexBuffer = RenderingContextDriver::instance()->createBuffer(m_vertices.size()* sizeof(Vertex), m_vertices.data(),
		BufferUsageBits::BUFFER_USAGE_TRANSFER_TO_BIT | BufferUsageBits::BUFFER_USAGE_VERTEX_BIT);
}

void Mesh::freeVertexBuffer()
{
	RenderingContextDriver::instance()->freeBuffer(m_vertexBuffer);
}

void Mesh::createIndexBuffer()
{
	if (m_indices.empty())
	{
		return;
	}

	m_indexBuffer = RenderingContextDriver::instance()->createBuffer(m_indices.size() * sizeof(uint32_t), m_indices.data(),
		BufferUsageBits::BUFFER_USAGE_TRANSFER_TO_BIT | BufferUsageBits::BUFFER_USAGE_INDEX_BIT);
}

void Mesh::freeIndexBuffer()
{
	RenderingContextDriver::instance()->freeBuffer(m_indexBuffer);
}

void Mesh::record()
{
	auto commandGroup = std::make_shared<CommandGroup>();
	commandGroup->push(std::make_shared<BindBufferCommand>(m_vertexBuffer, BufferType::Vertex));
	commandGroup->push(std::make_shared<BindBufferCommand>(m_indexBuffer, BufferType::Index));

	switch (m_drawMode)
	{
	case DrawMode::Indexed:
		commandGroup->push(std::make_shared<DrawCommand>(DrawMode::Indexed, static_cast<uint32_t>(m_indices.size()), 1, 0, 0, 0));
		break;
	case DrawMode::NonIndexed:
		commandGroup->push(std::make_shared<DrawCommand>(DrawMode::NonIndexed, static_cast<uint32_t>(m_vertices.size()), 1, 0, 0, 0 ));
		break;
	default:
		break;
	}

	FrameManager::instance()->currentCommandGroup()->push(commandGroup);
}
