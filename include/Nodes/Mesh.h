#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Nodes/Exports.h"

enum class DrawMode {
    Indexed,    // 使用索引绘制
    NonIndexed  // 非索引绘制
};

enum class PrimitiveType {
    Triangles,  // 三角形
    Lines,      // 线段
    Points      // 点
};

struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;
};

class NODES_API Mesh
{
public:
    Mesh();

    DrawMode m_drawMode;
    PrimitiveType m_primitiveType;

    std::vector<double> m_vertices;
    std::vector<uint32_t> m_indices;

    bool isChanged();
    void setChanged(const bool value);

private:
    bool m_isChanged;
};

