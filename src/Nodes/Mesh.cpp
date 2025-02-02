#include "Nodes/Mesh.h"

Mesh::Mesh()
	:m_drawMode(DrawMode::Indexed)
	,m_isChanged(true)
	,m_primitiveType(PrimitiveType::Triangles)
{
}

bool Mesh::isChanged()
{
	return m_isChanged;
}

void Mesh::setChanged(const bool value)
{
	m_isChanged = value;
}
