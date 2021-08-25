#pragma once
#include "Trace.h"

#include <string>
#include <string_view>

namespace trbl {
class Tracer
{
  public:
    Tracer(std::string_view name);
    ~Tracer();

    void stop();

  private:
    std::string name_{};
    TraceId parent_{};
    TraceId id_{};
    TimeStamp start_{};
    TimeStamp end_{};
    bool running_{true};
};

}  // namespace trbl
