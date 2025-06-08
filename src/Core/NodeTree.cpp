#include "Core/NodeTree.h"

void NodeTree::addNode(std::shared_ptr<Node> node)
{
	m_nodes.emplace_back(node);
}

uint32_t NodeTree::size() const
{
	return m_nodes.size();
}
