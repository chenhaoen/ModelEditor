#pragma once

#include <map>

#include "RenderingContextDriver/Commons.h"
#include "RenderingContextDriver/Exports.h"

class RENDERINGCONTEXTDRIVER_API PipelineManager
{
public:
	static PipelineManager* instance();

	static void destroy();

	PipelineID getPipeline(const PipelineType type);

private:
	PipelineManager();
	~PipelineManager();

	void init();

	void addPipeline(const PipelineType type, PipelineID pipeline);

	static PipelineManager* g_instance;

	std::map<PipelineType, PipelineID> m_pipelines;
};

