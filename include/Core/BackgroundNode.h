#pragma once

#include "Core/Node.h"
#include "Core/Exports.h"

class CORE_API BackgroundNode :
    public Node
{
public:
    BackgroundNode();
    ~BackgroundNode() override;

    void record() override;
    void compile() override;
};

