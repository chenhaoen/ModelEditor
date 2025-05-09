#pragma once

#include "IO/Exports.h"

class Node;
class SkyboxNode;

IO_API std::shared_ptr<Node> readNode(const std::string_view& file);
IO_API std::shared_ptr<SkyboxNode> readSkyboxNode(const std::string_view& file);

