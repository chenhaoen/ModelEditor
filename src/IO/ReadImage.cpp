#include "IO/ReadImage.h"
#include "IO/stb_image.h"

#include "Core/Image.h"



static_assert(std::is_same< stbi_uc, unsigned char>::value);

std::shared_ptr<Image> readImage(const std::string_view& file)
{
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(file.data(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

    if (!pixels) {
        return nullptr;
    }

    auto image = std::make_shared<Image>();
    image->setHeight(texHeight);
    image->setWidth(texWidth);
    image->setChannels(4);
    image->setDatas(pixels);

	return image;
}

void readKtx()
{
    
}
