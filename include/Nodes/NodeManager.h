#pragma once

#include "Nodes/Exports.h"

class Mesh;
class NODES_API NodeManager
{
public:
	static Mesh* root();
private:
	static Mesh* g_root;
};

