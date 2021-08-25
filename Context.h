#pragma once
#include "Trace.h"

#include <thread>
namespace trbl {
struct Context
{
    TraceId activeTrace{INVALID_TRACE_ID};
    std::thread::id threadId{std::this_thread::get_id()};
};

Context& getContext();

}  // namespace trbl
