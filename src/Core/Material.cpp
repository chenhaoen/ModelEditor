#include "Core/Material.h"
#include "Core/Image.h"
#include "Core/FrameManager.h"
#include "Core/PipelineManager.h"
#include "Core/Pipeline.h"

#include "Core/RenderingContextDriver/RenderingContextDriver.h"

Material::Material()
{
}

Material::~Material()
{
	freeTexture();
}

void Material::setTexturePath(const std::string& path)
{
	m_texturePath = path;
}

const std::string& Material::getTexturePath() const
{
	return m_texturePath;
}

void Material::setImage(std::shared_ptr<Image> image)
{
	m_image = image;
}

void Material::record()
{
	BoundUniform uniform;
	uniform.type = UNIFORM_TYPE_SAMPLER_WITH_TEXTURE;
	uniform.ids.push_back(m_texureID);
	uniform.binding = 1;
	PipelineManager::instance()->currentPipeline()->m_boundUniforms.push_back(uniform);
}

void Material::createTexture()
{
	if (!m_image)
	{
		return;
	}

	m_texureID = RenderingContextDriver::instance()->createTexture(m_image->width(), m_image->height(), m_image->channels(), m_image->datas());
}

void Material::freeTexture()
{
	RenderingContextDriver::instance()->freeTexture(m_texureID);
}

void Material::compile()
{
	if (m_compiled)
	{
		return;
	}

	createTexture();

	m_compiled = true;
}
