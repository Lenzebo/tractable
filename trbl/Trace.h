#pragma once
#include "Backend.h"
#include "Context.h"
#include "Span.h"

#include <string>
#include <string_view>

namespace trbl {

template <typename ClockT>
class TraceT
{
  public:
    TraceT(std::string_view name, Level l = Level::HIGH) : name_(name)
    {
        if (getLevel() <= l)
        {
            running_ = true;
            span_.parent = getContext().activeTrace;
            span_.id = std::hash<std::string_view>{}(name);
            span_.start = ClockT::now();
            getContext().activeTrace = span_.id;
        }
    }

    ~TraceT()
    {
        if (running_) stop();
    }

    void stop()
    {
        assert(running_);
        running_ = false;
        span_.end = ClockT::now();
        getContext().activeTrace = span_.parent;
        getBackend().writeSpan(name_, span_);
    }

  private:
    std::string_view name_{};
    Span span_{};
    bool running_{false};
};

using Trace = TraceT<std::chrono::high_resolution_clock>;

}  // namespace trbl
