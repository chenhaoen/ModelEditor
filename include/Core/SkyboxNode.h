#pragma once

#include "Core/Node.h"
#include "Core/Exports.h"

class CORE_API SkyboxNode :
    public Node
{
public:
    SkyboxNode();
    ~SkyboxNode() override;

    void record() override;
    void compile() override;

};

