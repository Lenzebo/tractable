#pragma once

#include <chrono>

namespace trbl {

using TimeStamp = std::chrono::time_point<std::chrono::high_resolution_clock>;
using TraceId = uint64_t;
constexpr inline TraceId INVALID_TRACE_ID = std::numeric_limits<TraceId>::max();

struct Trace
{
    TimeStamp start{};
    TimeStamp end{};
    TraceId id{INVALID_TRACE_ID};
    TraceId parent{INVALID_TRACE_ID};
};

}  // namespace trbl
