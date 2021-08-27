#pragma once
#include <variant>

namespace trbl {

using Attribute = std::variant<int32_t, int64_t, uint64_t, float, double, std::string, const char*, std::string_view>;

}