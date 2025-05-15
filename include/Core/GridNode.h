#pragma once

#include "Core/Node.h"

class GridNode :
    public Node
{
public:
    GridNode();

    void record() override;
    void compile() override;
};

