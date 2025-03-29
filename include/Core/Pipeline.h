#pragma once

#include <memory>

#include "RenderingContextDriver/Commons.h"

class CommandGroup;
class Pipeline
{
public:

	Pipeline(PipelineType type);
	~Pipeline();

	void bind();

	std::shared_ptr<CommandGroup> getCommands();

	void updateDescriptorSets();

	PipelineID m_id;

	std::vector<BoundUniform> m_boundUniforms;

	PipelineType m_type;
};

