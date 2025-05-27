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
        int gridSize,               // �����С (N x N)
        float cellSize,             // ÿ����Ԫ��Ĵ�С
        std::vector<Vertex>& vertices,  // ����Ķ�������
        std::vector<uint32_t>& indices, // �������������
        bool includeDiagonals = false  // �Ƿ�����Խ���
    );

private:
    BufferID m_vertexBuffer;
    BufferID m_indexBuffer;
};

