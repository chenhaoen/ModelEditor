#include "Core/Mesh.h"
#include "Core/Image.h"

#include "RenderingContextDriver/RenderingContextDriver.h"
#include "RenderingContextDriver/FrameManager.h"

Mesh::Mesh()
	: m_drawMode(DrawMode::Indexed)
	, m_isChanged(true)
	, m_isCompiled(false)
	, m_primitiveType(PrimitiveType::Triangles)
	, m_image(nullptr)
{
}

Mesh::~Mesh()
{
	freeVertexBuffer();
	freeIndexBuffer();
	freeTexture();
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

void Mesh::setImage(Image* image)
{
	m_image = image;
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
	createTexture();

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

void Mesh::createTexture()
{
	if (!m_image)
	{
		return;
	}

	m_texureID = RenderingContextDriver::instance()->createTexture(m_image->width(), m_image->height(), m_image->channels(), m_image->datas());

	BoundUniform uniform;
	uniform.type = UNIFORM_TYPE_SAMPLER_WITH_TEXTURE;
	uniform.ids.push_back(m_texureID);
	FrameManager::instance()->addBoundUniform(uniform);
}

void Mesh::freeTexture()
{
	RenderingContextDriver::instance()->freeTexture(m_texureID);
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
