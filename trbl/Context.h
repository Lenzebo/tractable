#pragma once
#include "Span.h"

#include <thread>
namespace trbl {
struct Context
{
    SpanId activeTrace{INVALID_SPAN_ID};
    std::thread::id threadId{std::this_thread::get_id()};
};

Context& getContext();

}  // namespace trbl
