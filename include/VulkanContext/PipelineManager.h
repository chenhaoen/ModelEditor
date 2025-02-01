#pragma once

#include <string_view>
#include <map>

class Pipeline;

class PipelineManager
{
public:
	static PipelineManager* instance();

	void destroy();

	void AddPipeline(const std::string_view& name, Pipeline* pipeline);
private:
	PipelineManager();

	static PipelineManager* g_instance;

	std::map<std::string_view, Pipeline*> m_pipelines;
};

