#pragma

#include "Core/Exports.h"

class CORE_API Image
{
public:
	Image();

	void setWidth(uint32_t);
	uint32_t width() const ;

	void setHeight(uint32_t);
	uint32_t height() const;

	void setChannels(uint32_t);
	uint32_t channels() const;

	uint32_t size() const;

	void setDatas(unsigned char* datas);
	const unsigned char* datas() const;
	unsigned char* datas();

	void allocale();
private:
	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_channels;
	uint32_t m_size;

	unsigned char* m_datas;
};