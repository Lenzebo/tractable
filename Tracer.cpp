#include "Tracer.h"

#include "Backend.h"
#include "Context.h"

namespace trbl {
Tracer::Tracer(std::string_view name)
    : name_(name),
      parent_(getContext().activeTrace),
      id_(std::hash<std::string_view>{}(name)),
      start_(std::chrono::high_resolution_clock::now())
{
    getContext().activeTrace = id_;
}

Tracer::~Tracer()
{
    if (running_) stop();
    getBackend().writeTrace(name_, Trace{start_, end_, id_, parent_});
}

void Tracer::stop()
{
    assert(running_);
    running_ = false;
    end_ = std::chrono::high_resolution_clock::now();
    getContext().activeTrace = parent_;
}
}  // namespace trbl