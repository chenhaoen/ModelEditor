#pragma once

#include <string_view>
#include <map>

class Pipeline;

class PipelineManager
{
public:
	static PipelineManager* instance();

	void init();

	void destroy();

	Pipeline* getPipeline(const std::string_view& name);

private:
	PipelineManager();
	~PipelineManager();

	void addPipeline(const std::string_view& name, Pipeline* pipeline);

	static PipelineManager* g_instance;

	std::map<std::string_view, Pipeline*> m_pipelines;
};

