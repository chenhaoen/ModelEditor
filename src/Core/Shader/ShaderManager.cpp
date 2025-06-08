#include "Core/Shader/ShaderManager.h"
#include "Core/Shader/Shader.h"
#include "Core/Shader/Descriptor.h"

ShaderManager* ShaderManager::g_instance = nullptr;

ShaderManager* ShaderManager::instance()
{
	if (!g_instance)
	{
		g_instance = new ShaderManager();
	}

	return g_instance;
}

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::init()
{
	// Model
	{
		auto shader = std::make_shared<Shader>();
		shader->setFileName("E:/code/ModelEditer/build/bin/Debug/shaders/vert.spv");
		shader->setfuncName("main");
		shader->setType(ShaderType::Vertex);

		auto descriptor = std::make_shared<Descriptor>();
		descriptor->setBinding(0);
		descriptor->setUniformType(UniformType::UNIFORM_TYPE_UNIFORM_BUFFER);
		shader->addDescriptor(descriptor);

		m_shaders.emplace("Model.vert", shader);
	}

	{
		auto shader = std::make_shared<Shader>();
		shader->setFileName("E:/code/ModelEditer/build/bin/Debug/shaders/frag.spv");
		shader->setfuncName("main");
		shader->setType(ShaderType::Fragment);

		auto descriptor = std::make_shared<Descriptor>();
		descriptor->setBinding(1);
		descriptor->setUniformType(UniformType::UNIFORM_TYPE_SAMPLER_WITH_TEXTURE);
		shader->addDescriptor(descriptor);

		m_shaders.emplace("Model.frag", shader);
	}

	// sky box
	{
		auto shader = std::make_shared<Shader>();
		shader->setFileName("E:/code/ModelEditer/build/bin/Debug/shaders/skyboxVert.spv");
		shader->setfuncName("main");
		shader->setType(ShaderType::Vertex);

		auto descriptor = std::make_shared<Descriptor>();
		descriptor->setBinding(0);
		descriptor->setUniformType(UniformType::UNIFORM_TYPE_UNIFORM_BUFFER);
		shader->addDescriptor(descriptor);

		m_shaders.emplace("skybox.vert", shader);
	}

	{
		auto shader = std::make_shared<Shader>();
		shader->setFileName("E:/code/ModelEditer/build/bin/Debug/shaders/skyboxFrag.spv");
		shader->setfuncName("main");
		shader->setType(ShaderType::Fragment);

		auto descriptor = std::make_shared<Descriptor>();
		descriptor->setBinding(1);
		descriptor->setUniformType(UniformType::UNIFORM_TYPE_SAMPLER_WITH_TEXTURE);
		shader->addDescriptor(descriptor);

		m_shaders.emplace("skybox.frag", shader);
	}

	// grids
	{
		auto shader = std::make_shared<Shader>();
		shader->setFileName("E:/code/ModelEditer/build/bin/Debug/shaders/gridsCompute.spv");
		shader->setfuncName("main");
		shader->setType(ShaderType::Compute);

		{
			auto descriptor = std::make_shared<Descriptor>();
			descriptor->setBinding(0);
			descriptor->setUniformType(UniformType::UNIFORM_TYPE_STORAGE_BUFFER);
			shader->addDescriptor(descriptor);
		}

		{
			auto descriptor = std::make_shared<Descriptor>();
			descriptor->setBinding(1);
			descriptor->setUniformType(UniformType::UNIFORM_TYPE_STORAGE_BUFFER);
			shader->addDescriptor(descriptor);
		}

		m_shaders.emplace("grids.compute", shader);
	}

	{
		auto shader = std::make_shared<Shader>();
		shader->setFileName("E:/code/ModelEditer/build/bin/Debug/shaders/gridsVert.spv");
		shader->setfuncName("main");
		shader->setType(ShaderType::Vertex);

		auto descriptor = std::make_shared<Descriptor>();
		descriptor->setBinding(0);
		descriptor->setUniformType(UniformType::UNIFORM_TYPE_UNIFORM_BUFFER);
		shader->addDescriptor(descriptor);

		m_shaders.emplace("grids.vert", shader);
	}

	{
		auto shader = std::make_shared<Shader>();
		shader->setFileName("E:/code/ModelEditer/build/bin/Debug/shaders/gridsFrag.spv");
		shader->setfuncName("main");
		shader->setType(ShaderType::Fragment);

		m_shaders.emplace("grids.frag", shader);
	}

	{
		auto shader = std::make_shared<Shader>();
		shader->setFileName("E:/code/ModelEditer/build/bin/Debug/shaders/backgroundVert.spv");
		shader->setfuncName("main");
		shader->setType(ShaderType::Vertex);

		m_shaders.emplace("background.vert", shader);
	}

	{
		auto shader = std::make_shared<Shader>();
		shader->setFileName("E:/code/ModelEditer/build/bin/Debug/shaders/backgroundFrag.spv");
		shader->setfuncName("main");
		shader->setType(ShaderType::Fragment);

		m_shaders.emplace("background.frag", shader);
	}
}

std::shared_ptr<Shader> ShaderManager::getShader(const std::string_view& shaderName)
{
	if (!m_shaders.count(shaderName))
	{
		std::cerr << "No shader!";
	}

	return m_shaders.at(shaderName);
}
