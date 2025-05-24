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

private:
    BufferID m_vertexBuffer;
    BufferID m_indexBuffer;
};

