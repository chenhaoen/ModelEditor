#pragma once

#include "IO/Exports.h"

class Image;
IO_API std::shared_ptr<Image> readImage(const std::string_view& file);

void readKtx();

