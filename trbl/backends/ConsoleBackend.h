#pragma once

#include "trbl/Backend.h"

#include <fstream>

namespace trbl::backends {
class ConsoleBackend : public Backend
{
  public:
    void writeSpan(const std::string_view name, const Span& span, const Context& context) override;
    void writeCounter(const std::string_view name, const CounterEvent event, const Context& context) override;
    void writeEvent(const std::string_view name, const TimeStamp ts, const Context& context) override;
};
}  // namespace trbl::backends
