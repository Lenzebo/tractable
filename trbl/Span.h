#pragma once

#include <chrono>

namespace trbl {

using TimeStamp = std::chrono::time_point<std::chrono::high_resolution_clock>;
using SpanId = uint64_t;
constexpr inline SpanId INVALID_SPAN_ID = std::numeric_limits<SpanId>::max();

struct Span
{
    TimeStamp start{};
    TimeStamp end{};
    SpanId id{INVALID_SPAN_ID};
    SpanId parent{INVALID_SPAN_ID};
};

}  // namespace trbl
