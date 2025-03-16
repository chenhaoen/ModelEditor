#include "Core/Material.h"
#include "Core/Image.h"

#include "RenderingContextDriver/RenderingContextDriver.h"
#include "RenderingContextDriver/FrameManager.h"

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

	// TO DO 这里以后要改
	compile();
}

void Material::record()
{
	compile();
}

void Material::createTexture()
{
	if (!m_image)
	{
		return;
	}

	m_texureID = RenderingContextDriver::instance()->createTexture(m_image->width(), m_image->height(), m_image->channels(), m_image->datas());

	BoundUniform uniform;
	uniform.type = UNIFORM_TYPE_SAMPLER_WITH_TEXTURE;
	uniform.ids.push_back(m_texureID);
	FrameManager::instance()->addBoundUniform(uniform);
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
