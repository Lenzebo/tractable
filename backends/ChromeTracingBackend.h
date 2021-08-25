#pragma once

#include "../Backend.h"

#include <fstream>

namespace trbl::backends {
class ChromeTracingBackend : public Backend
{
  public:
    void beginTracing() override;
    void endTracing() override;
    void writeTrace(const std::string_view name, const Trace& trace, const Context& context) override;

  private:
    std::ofstream outFile_;
    bool first_{true};
};
}  // namespace trbl::backends
