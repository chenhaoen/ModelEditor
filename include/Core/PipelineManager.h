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

	std::shared_ptr<Pipeline> getPipeline(const PipelineType type);

	uint32_t pipelineCount() const;

	std::shared_ptr<CommandGroup> getCommands(const PipelineType type);

	void updateDescriptorSets(const PipelineType type);
private:
	PipelineManager();
	~PipelineManager();

	void addPipeline(const PipelineType type, std::shared_ptr<Pipeline> pipeline);

	static PipelineManager* g_instance;

	std::map<PipelineType, std::shared_ptr<Pipeline>> m_pipelines;
};

