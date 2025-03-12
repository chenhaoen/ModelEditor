#pragma once

#include <string_view>

#include "IO/Exports.h"

IO_API class Image* readImage(const std::string_view& file);

