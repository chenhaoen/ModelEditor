#pragma once

#include <map>
#include <memory>

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

	void setCurrentPipeline(const PipelineType type);
	std::shared_ptr<Pipeline> currentPipeline();
	std::shared_ptr<Pipeline> getPipeline(const PipelineType type);

	uint32_t pipelineCount() const;

	std::shared_ptr<CommandGroup> getCommands();

	void updateDescriptorSets();
private:
	PipelineManager();
	~PipelineManager();

	void addPipeline(const PipelineType type, std::shared_ptr<Pipeline> pipeline);

	static PipelineManager* g_instance;

	std::map<PipelineType, std::shared_ptr<Pipeline>> m_pipelines;

	PipelineType m_currentPipeline;
};

