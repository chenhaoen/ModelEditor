#pragma once

#include "Core/Exports.h"

class Mesh;
class CORE_API NodeManager
{
public:
	static Mesh* root();
	static void destoryRoot();
private:
	static Mesh* g_root;
};

