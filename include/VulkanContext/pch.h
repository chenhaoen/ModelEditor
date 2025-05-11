#pragma once

#include <string_view>
#include <array>
#include <vector>
#include <stdexcept>
#include <memory>
#include <iostream>
#include <set>
#include <chrono>
#include <fstream>
#include <list>

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

#include <ktxvulkan.h>

#ifdef _WIN64
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#endif // _WIN64
