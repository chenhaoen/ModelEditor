#pragma once

#include "Core/RenderingContextDriver/Commons.h"

#include "Core/Exports.h"

class Pipeline;
class CommandGroup;
class CORE_API PipelineManager
{
public:
	static PipelineManager* instance();

	static void destroy();

	void init();

	std::shared_ptr<Pipeline> getPipeline(const PipelineName);

	uint32_t pipelineCount() const;

	std::shared_ptr<CommandGroup> getCommands(const PipelineName);

	void updateDescriptorSets(const PipelineName);
private:
	PipelineManager();
	~PipelineManager();

	void addPipeline(const PipelineName type, std::shared_ptr<Pipeline> pipeline);

	static PipelineManager* g_instance;

	std::map<PipelineName, std::shared_ptr<Pipeline>> m_pipelines;
};

