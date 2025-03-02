#include "Nodes/NodeManager.h"
#include "Nodes/Mesh.h"

Mesh* NodeManager::g_root = nullptr;

const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};

const std::vector<uint32_t> indices = {
    0, 1, 2, 2, 3, 0
};

Mesh* NodeManager::root()
{
    if (!g_root)
    {
        g_root = new Mesh();

        g_root->m_drawMode = DrawMode::Indexed;
        g_root->setVertices(vertices);
        g_root->setIndices(indices);
    }

    return g_root;
}

void NodeManager::destoryRoot()
{
    delete g_root;
    g_root = nullptr;
}
