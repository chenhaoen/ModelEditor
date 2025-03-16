#pragma once

#include <string_view>
#include <memory>

#include "IO/Exports.h"

class Node;
IO_API std::shared_ptr<Node> readNode(const std::string_view& file);

