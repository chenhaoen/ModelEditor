#include "Core/Image.h"

Image::Image()
	:m_width{0}
	,m_height{0}
	,m_datas{nullptr}
	,m_channels(4)
	,m_size(0)
{
}

void Image::setWidth(uint32_t value)
{
	m_width = value;
}

uint32_t Image::width() const
{
	return m_width;
}

void Image::setHeight(uint32_t value)
{
	m_height = value;
}

void Image::setDatas(unsigned char* datas)
{
	m_datas = datas;
}

const unsigned char* Image::datas() const
{
	return m_datas;
}

unsigned char* Image::datas()
{
	return m_datas;
}

void Image::allocale()
{
}

uint32_t Image::height() const
{
	return m_height;
}

void Image::setChannels(uint32_t value)
{
	m_channels = value;
}

uint32_t Image::channels() const
{
	return m_channels;
}

uint32_t Image::size() const
{
	return m_channels * m_width * m_height;
}
