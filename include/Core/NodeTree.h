#pragma once

#include "Core/Exports.h"

class Node;
class CORE_API NodeTree
{
public:
	void addNode(std::shared_ptr<Node>);

	uint32_t size() const;
private:
	std::list<std::shared_ptr<Node>> m_nodes;
};