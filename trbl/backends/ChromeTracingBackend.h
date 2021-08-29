#pragma once

#include "trbl/Backend.h"

#include <fstream>

namespace trbl::backends {
class ChromeTracingBackend : public Backend
{
  public:
    ChromeTracingBackend(std::string filename = "profile.json");

    void beginTracing() override;
    void endTracing() override;
    void writeSpan(const std::string_view name, const Span& span, const Context& context) override;
    void writeCounter(const std::string_view name, const CounterEvent event, const Context& context) override;
    void writeEvent(const std::string_view name, const TimeStamp ts, const Context& context) override;

  private:
    std::string filename_;
    std::ofstream outFile_;
    bool first_{true};
};
}  // namespace trbl::backends
