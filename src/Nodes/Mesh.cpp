#include "Nodes/Mesh.h"

#include "RenderingContextDriver/RenderingContextDriver.h"
#include "RenderingContextDriver/FrameManager.h"

Mesh::Mesh()
	: m_drawMode(DrawMode::Indexed)
	, m_isChanged(true)
	, m_isCompiled(false)
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

const std::vector<Vertex>& Mesh::getVertices() const
{
	return m_vertices;
}

void Mesh::setIndices(const std::vector<uint32_t>& indices)
{
	m_indices = indices;
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
	if (m_isCompiled)
	{
		return;
	}

	createVertexBuffer();
	createIndexBuffer();

	m_isCompiled = true;
}

void Mesh::createVertexBuffer()
{
	if (m_vertices.empty())
	{
		return;
	}

	m_vertexBuffer = RenderingContextDriver::instance()->createVertexBuffer(m_vertices);
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

	m_indexBuffer = RenderingContextDriver::instance()->createIndexBuffer(m_indices);
}

void Mesh::freeIndexBuffer()
{
	RenderingContextDriver::instance()->freeBuffer(m_indexBuffer);
}

void Mesh::record()
{
	compile();

	CommandBufferID currentCommandBuffer = FrameManager::instance()->currentCommandBuffer();

	RenderingContextDriver::instance()->cmdBindVertexBuffer(currentCommandBuffer, m_vertexBuffer);

	RenderingContextDriver::instance()->cmdBindIndexBuffer(currentCommandBuffer, m_indexBuffer);

	switch (m_drawMode)
	{
	case DrawMode::Indexed:
		RenderingContextDriver::instance()->cmdDrawIndexed(currentCommandBuffer, static_cast<uint32_t>(m_indices.size()), 1, 0, 0, 0);
		break;
	case DrawMode::NonIndexed:
		RenderingContextDriver::instance()->cmdDraw(currentCommandBuffer, 3, 1, 0, 0);
		break;
	default:
		break;
	}

}
