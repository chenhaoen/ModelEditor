#pragma once

#include "Core/RenderingContextDriver/Commons.h"
#include "Core/Pipeline/PipelineCreateInfo.h"

#include "Core/FrameManager.h"
#include "Core/DynamicState.h"

class CommandGroup;
class Pipeline
{
public:

	Pipeline(const PipelineCreateInfo& pipelineCreateInfo);
	~Pipeline();

	void bind();

	std::shared_ptr<CommandGroup> getCommands();

	void updateDescriptorSets();

	PipelineID m_id;

	std::vector<BoundUniform> m_boundUniforms;

	std::list<std::shared_ptr<DynamicState>> m_dynamicStates;

	std::array<UniformSetID, FrameManager::maxFrameCount()> m_descriptorSets;
};

