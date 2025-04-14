#pragma once

#include <string>
#include <memory>

#include "Core/RenderingContextDriver/Commons.h"

#include "Core/Exports.h"
#include "Core/Compilable.h"

class Image;
class CORE_API Material : public Compilable
{
public:
    Material();
    ~Material() override;

    void setTexturePath(const std::string& path);

    const std::string& getTexturePath() const;

    void setImage(std::shared_ptr<Image> image);

    void record() override;

    void compile() override;

    TextureID m_texureID;
private:
    void createTexture();

    void freeTexture();
private:
    std::string m_texturePath;

    std::shared_ptr<Image> m_image;
};

