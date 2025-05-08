#pragma once

#include <memory>
#include <array>

#include "Core/RenderingContextDriver/Commons.h"

#include "Core/FrameManager.h"

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

	std::array<UniformSetID, FrameManager::maxFrameCount()> m_descriptorSets;

	PipelineType m_type;
};

