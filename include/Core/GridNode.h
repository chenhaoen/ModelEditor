#pragma once

#include "Core/Node.h"
#include "Core/Exports.h"

#include "Core/RenderingContextDriver/Commons.h"

class CORE_API GridNode :
    public Node
{
public:
    GridNode();
    ~GridNode();

    void record() override;
    void compile() override;

    void GenerateGridMesh(
        int gridSize,               // 网格大小 (N x N)
        float cellSize,             // 每个单元格的大小
        std::vector<Vertex>& vertices,  // 输出的顶点数据
        std::vector<uint32_t>& indices, // 输出的索引数据
        bool includeDiagonals = false  // 是否包含对角线
    );

private:
    BufferID m_vertexBuffer;
    BufferID m_indexBuffer;
};

