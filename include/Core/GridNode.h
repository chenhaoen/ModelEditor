#pragma once

#include "Core/Node.h"

#include "Core/Exports.h"

class CORE_API GridNode :
    public Node
{
public:
    GridNode();

    void record() override;
    void compile() override;
};

