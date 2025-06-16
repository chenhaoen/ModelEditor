#include "Core/NodeTree.h"
#include "Core/Node.h"

void NodeTree::addNode(std::shared_ptr<Node> node)
{
	m_nodes.emplace_back(node);
}

uint32_t NodeTree::size() const
{
	return m_nodes.size();
}

void NodeTree::record()
{
	for (auto node : m_nodes)
	{
		node->record();
	}
}

void NodeTree::compile()
{
	for (auto node : m_nodes)
	{
		node->compile();
	}
}
