#include "Nodes/NodeManager.h"
#include "Nodes/Mesh.h"

Mesh* NodeManager::g_root = nullptr;

Mesh* NodeManager::root()
{
    if (!g_root)
    {
        g_root = new Mesh();

        g_root->m_drawMode = DrawMode::NonIndexed; 
    }

    return g_root;
}
